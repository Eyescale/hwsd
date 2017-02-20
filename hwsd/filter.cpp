
/* Copyright (c) 2011-2013, Stefan Eilemann <eile@eyescale.ch>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 2.1 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "filter.h"
#include "gpuInfo.h"
#include "netInfo.h"

#include <boost/regex.hpp>

#ifdef HWSD_USE_QT5NETWORK
#include <QtNetwork/QHostAddress>
#endif

#include <algorithm>

namespace hwsd
{
/** @cond IGNORE */
typedef std::vector<FilterPtr> Filters;
typedef Filters::const_iterator FiltersCIter;
/** @endcond */

// Filter
//-------
namespace detail
{
class Filter
{
public:
    Filters next_;
};
}

Filter::Filter()
    : impl_(new detail::Filter)
{
}
Filter::~Filter()
{
    delete impl_;
}

bool Filter::operator()(const GPUInfos& current, const GPUInfo& candidate)
{
    for (FiltersCIter i = impl_->next_.begin(); i != impl_->next_.end(); ++i)
    {
        FilterPtr filter = *i;
        if (!(*filter)(current, candidate))
            return false;
    }
    return true;
}

bool Filter::operator()(const NetInfos& current, const NetInfo& candidate)
{
    for (FiltersCIter i = impl_->next_.begin(); i != impl_->next_.end(); ++i)
    {
        FilterPtr filter = *i;
        if (!(*filter)(current, candidate))
            return false;
    }
    return true;
}

FilterPtr Filter::operator|(FilterPtr rhs)
{
    impl_->next_.push_back(rhs);
    return this;
}

FilterPtr Filter::operator|=(FilterPtr rhs)
{
    impl_->next_.push_back(rhs);
    return this;
}

// DuplicateFilter
//----------------
bool DuplicateFilter::operator()(const GPUInfos& current,
                                 const GPUInfo& candidate)
{
    GPUInfosCIter i = std::find(current.begin(), current.end(), candidate);
    if (i == current.end())
        return Filter::operator()(current, candidate);

    // TODO: break API in 2.0, making current non-const?
    GPUInfo& info = const_cast<GPUInfo&>(*i);
    info.flags |= candidate.flags; // merge flags from dropped info
    return false;
}

bool DuplicateFilter::operator()(const NetInfos& current,
                                 const NetInfo& candidate)
{
    NetInfosCIter i = std::find(current.begin(), current.end(), candidate);
    if (i == current.end())
        return Filter::operator()(current, candidate);
    return false;
}

// MirrorFilter
//-------------
bool MirrorFilter::operator()(const GPUInfos& current, const GPUInfo& candidate)
{
    for (GPUInfosCIter i = current.begin(); i != current.end(); ++i)
    {
        const GPUInfo& info = *i;
        if (info.id == candidate.id && info.session == candidate.session &&
            info.port == candidate.port && info.device == candidate.device &&
            info.pvp[0] == candidate.pvp[0] && info.pvp[1] == candidate.pvp[1])
        {
            return false;
        }
    }

    return Filter::operator()(current, candidate);
}

// SessionFilter
//--------------
namespace detail
{
class SessionFilter
{
public:
    explicit SessionFilter(const std::string& name)
        : name_(name)
    {
    }

    const std::string name_;
};
}

SessionFilter::SessionFilter(const std::string& name)
    : impl_(new detail::SessionFilter(name))
{
}

SessionFilter::~SessionFilter()
{
    delete impl_;
}

bool SessionFilter::operator()(const GPUInfos& current,
                               const GPUInfo& candidate)
{
    if (impl_->name_.empty() || candidate.session == impl_->name_)
        return Filter::operator()(current, candidate);
    return false;
}

bool SessionFilter::operator()(const NetInfos& current,
                               const NetInfo& candidate)
{
    if (impl_->name_.empty() || candidate.session == impl_->name_)
        return Filter::operator()(current, candidate);
    return false;
}

// GPUFilter
//---------------
namespace detail
{
class GPUFilter
{
public:
    explicit GPUFilter(const std::string& regex_)
    {
#ifdef HWSD_USE_BOOST
        if (!regex_.empty())
            regex = regex_;
#endif
    }

#ifdef HWSD_USE_BOOST
    boost::regex regex;
#endif
};
}

GPUFilter::GPUFilter(const std::string& regex)
    : impl_(new detail::GPUFilter(regex))
{
#ifndef HWSD_USE_BOOST
    LBWARN << "Ignoring GPU regex filter, boost::regex not available"
           << std::endl;
#endif
}

GPUFilter::~GPUFilter()
{
    delete impl_;
}

bool GPUFilter::operator()(const hwsd::GPUInfos& current,
                           const hwsd::GPUInfo& candidate)
{
#ifdef HWSD_USE_BOOST
    std::ostringstream name;
    name << candidate.nodeName << ':' << candidate.port << '.'
         << candidate.device;

    if (impl_->regex.empty() || boost::regex_match(name.str(), impl_->regex))
#endif
        return Filter::operator()(current, candidate);

    return false;
}

// NetFilter
//---------------
namespace detail
{
class NetFilter
{
public:
    NetFilter(const lunchbox::Strings& prefixes_, const uint32_t type_)
        : prefixes(prefixes_)
        , type(type_)
    {
    }

    const lunchbox::Strings prefixes;
    const uint32_t type;
};
}

NetFilter::NetFilter(const lunchbox::Strings& prefixes, const uint32_t type)
    : impl_(new detail::NetFilter(prefixes, type))
{
}

NetFilter::~NetFilter()
{
    delete impl_;
}

bool NetFilter::operator()(const hwsd::NetInfos& current,
                           const hwsd::NetInfo& candidate)
{
    if (impl_->type != NetInfo::TYPE_ALL && !(impl_->type & candidate.type))
        return false;

#ifdef HWSD_USE_QT5NETWORK
    QHostAddress address4(QString::fromStdString(candidate.inetAddress));
    QHostAddress address6(QString::fromStdString(candidate.inet6Address));
    bool isInSubnet = impl_->prefixes.empty();
    for (lunchbox::StringsCIter j = impl_->prefixes.begin();
         j != impl_->prefixes.end(); ++j)
    {
        const QString prefixStr = QString::fromStdString(*j);
        const QPair<QHostAddress, int> subnet =
            QHostAddress::parseSubnet(prefixStr);
        if (address4.isInSubnet(subnet) || address6.isInSubnet(subnet))
        {
            isInSubnet = true;
            break;
        }
    }
#else
    if (!impl_->prefixes.empty())
        LBWARN << "Ignoring prefix filter, QtNetwork not available"
               << std::endl;
    const bool isInSubnet = true;
#endif

    if (isInSubnet)
        return Filter::operator()(current, candidate);

    return false;
}
}
