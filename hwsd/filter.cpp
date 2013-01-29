
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

#ifdef HWSD_USE_BOOST
#  include <boost/regex.hpp>
#endif

#include <algorithm>

namespace hwsd
{
/** @cond IGNORE */
typedef std::vector< FilterPtr > Filters;
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

Filter::Filter() : impl_( new detail::Filter ) {}
Filter::~Filter() { delete impl_; }

bool Filter::operator() ( const GPUInfos& current, const GPUInfo& candidate )
{
    for( FiltersCIter i = impl_->next_.begin(); i!=impl_->next_.end(); ++i)
    {
        FilterPtr filter = *i;
        if( !(*filter)( current, candidate ))
            return false;
    }
    return true;
}

bool Filter::operator() ( const NetInfos& current, const NetInfo& candidate )
{
    for( FiltersCIter i = impl_->next_.begin(); i!=impl_->next_.end(); ++i)
    {
        FilterPtr filter = *i;
        if( !(*filter)( current, candidate ))
            return false;
    }
    return true;
}

FilterPtr Filter::operator | ( FilterPtr rhs )
{
    impl_->next_.push_back( rhs );
    return rhs;
}

FilterPtr Filter::operator |= ( FilterPtr rhs )
{
    impl_->next_.push_back( rhs );
    return rhs;
}

// DuplicateFilter
//----------------
bool DuplicateFilter::operator() ( const GPUInfos& current,
                                   const GPUInfo& candidate )
{
    GPUInfosCIter i = std::find( current.begin(), current.end(), candidate );
    if( i == current.end())
        return Filter::operator()( current, candidate );

    // TODO: break API in 2.0, making current non-const?
    GPUInfo& info = const_cast< GPUInfo& >( *i );
    info.flags |= candidate.flags; // merge flags from dropped info
    return false;
}

bool DuplicateFilter::operator() ( const NetInfos& current,
                                   const NetInfo& candidate )
{
    NetInfosCIter i = std::find( current.begin(), current.end(), candidate );
    if( i == current.end())
        return Filter::operator()( current, candidate );
    return false;
}

// MirrorFilter
//-------------
bool MirrorFilter::operator() ( const GPUInfos& current,
                                const GPUInfo& candidate )
{
    for( GPUInfosCIter i = current.begin(); i != current.end(); ++i )
    {
        const GPUInfo& info = *i;
        if( info.id == candidate.id &&
            info.session == candidate.session &&
            info.device == candidate.device &&
            info.pvp[0] == candidate.pvp[0] && info.pvp[1] == candidate.pvp[1] )
        {
             return false;
        }
    }

    return Filter::operator()( current, candidate );
}

// SessionFilter
//--------------
namespace detail
{
class SessionFilter
{
public:
    SessionFilter( const std::string& name ) : name_( name ) {}

    const std::string& name_;
};
}

SessionFilter::SessionFilter( const std::string& name )
        : impl_( new detail::SessionFilter( name ))
{}

SessionFilter::~SessionFilter() { delete impl_; }

bool SessionFilter::operator() ( const GPUInfos& current,
                                 const GPUInfo& candidate )
{
    if( candidate.session == impl_->name_ )
        return Filter::operator()( current, candidate );
    return false;
}

bool SessionFilter::operator() ( const NetInfos& current,
                                 const NetInfo& candidate )
{
    if( candidate.session == impl_->name_ )
        return Filter::operator()( current, candidate );
    return false;
}

// GPUFilter
//---------------
namespace detail
{
class GPUFilter
{
public:
    GPUFilter( const std::string& regex_ )
#ifdef HWSD_USE_BOOST
        : regex( regex_ )
#endif
    {}

#ifdef HWSD_USE_BOOST
    boost::regex regex;
#endif
};
}

GPUFilter::GPUFilter( const std::string& regex )
    : impl_( new detail::GPUFilter( regex ))
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

bool GPUFilter::operator() ( const hwsd::GPUInfos& current,
                                  const hwsd::GPUInfo& candidate )
{
#ifdef HWSD_USE_BOOST
    std::ostringstream name;
    name << candidate.nodeName << ':' << candidate.port << '.'
         << candidate.device;

    if( boost::regex_match( name.str(), impl_->regex ))
#endif
        return Filter::operator()( current, candidate );

    return false;
}

}
