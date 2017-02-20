
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

#ifndef HWSD_FILTER_H
#define HWSD_FILTER_H

#include <hwsd/api.h>
#include <hwsd/types.h>

#include <lunchbox/referenced.h> // base class
#include <string>

namespace hwsd
{
namespace detail
{
class Filter;
class SessionFilter;
class GPUFilter;
class NetFilter;
}

/** Base class for all discovery filters. */
class Filter : public lunchbox::Referenced
{
public:
    /** Create a new filter. @version 1.0 */
    HWSD_API Filter();

    /** Destruct this filter. @version 1.0 */
    HWSD_API virtual ~Filter();

    /**
     * Chain another filter to this one.
     *
     * Invoking the operator() will call chained filters.
     * @version 1.0
     */
    HWSD_API FilterPtr operator|(FilterPtr rhs);

    /**
     * Chain another filter to this one.
     *
     * Invoking the operator() will call chained filters.
     * @version 1.0
     */
    HWSD_API FilterPtr operator|=(FilterPtr rhs);

    /**
     * Call all chained operators.
     *
     * Filter implementations overwrite this method to implement the filtering
     * and call this base class implementation if the candidate passed.
     *
     * @param current the list of passed GPU informations.
     * @param candidate the new GPU information to test.
     * @return true if all chained operators returned true, false otherwise.
     * @version 1.0
     */
    HWSD_API virtual bool operator()(const GPUInfos& current,
                                     const GPUInfo& candidate);

    HWSD_API virtual bool operator()(const NetInfos& current,
                                     const NetInfo& candidate);

private:
    detail::Filter* const impl_;
};

/** Chain two filters and return the chain. @version 1.0 */
inline FilterPtr operator|(FilterPtr a, FilterPtr b)
{
    return (*a) |= b;
}

/** Filters all duplicates during discovery. */
class DuplicateFilter : public Filter
{
public:
    virtual ~DuplicateFilter() {}
    /**
     * @return true if the candidate is not in the current vector.
     * @version 1.0
     */
    HWSD_API virtual bool operator()(const GPUInfos& current,
                                     const GPUInfo& candidate);

    HWSD_API virtual bool operator()(const NetInfos& current,
                                     const NetInfo& candidate);
};

/** Filter overlapping duplicates with different GPU types. */
class MirrorFilter : public Filter
{
public:
    virtual ~MirrorFilter() {}
    /**
     * @return true if the candidate is unique wrt the position, device,
     *         hostname and session.
     * @version 1.0
     */
    HWSD_API virtual bool operator()(const GPUInfos& current,
                                     const GPUInfo& candidate);
};

/** Filters for a specific session. */
class SessionFilter : public Filter
{
public:
    /** Matches the given name literally. An empty name matches all sessions. */
    HWSD_API explicit SessionFilter(const std::string& name);
    HWSD_API virtual ~SessionFilter();

    /** @return true if the candidate has the given session. @version 1.0 */
    HWSD_API virtual bool operator()(const GPUInfos& current,
                                     const GPUInfo& candidate);

    HWSD_API virtual bool operator()(const NetInfos& current,
                                     const NetInfo& candidate);

private:
    detail::SessionFilter* const impl_;
};

/** Filters for a specific GPU regex. */
class GPUFilter : public Filter
{
public:
    /**
     * Matches the GPU agaings the given regex. An empty regex matches all
     * GPUs.
     */
    HWSD_API explicit GPUFilter(const std::string& regex);
    HWSD_API virtual ~GPUFilter();

    /**
     * @return true of the regex matches 'nodename:port.device'
     * @version 1.0
     */
    HWSD_API virtual bool operator()(const hwsd::GPUInfos& current,
                                     const hwsd::GPUInfo& candidate);

private:
    detail::GPUFilter* const impl_;
};

/** Filter for network interfaces matching prefixes and/or type. */
class NetFilter : public Filter
{
public:
    HWSD_API NetFilter(const lunchbox::Strings& prefixes, const uint32_t type);
    HWSD_API virtual ~NetFilter();

    /**
     * @return true if matching prefix and type
     * @version 1.0
     */
    HWSD_API virtual bool operator()(const hwsd::NetInfos& current,
                                     const hwsd::NetInfo& candidate);

private:
    detail::NetFilter* const impl_;
};
}
#endif // HWSD_FILTER_H
