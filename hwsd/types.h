
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

#ifndef HWSD_TYPES_H
#define HWSD_TYPES_H

#include <hwsd/module.h>

#include <lunchbox/refPtr.h>
#include <vector>

namespace hwsd
{
struct GPUInfo;
typedef std::vector<GPUInfo> GPUInfos;   //!< A vector of GPUInfo structs
typedef GPUInfos::iterator GPUInfosIter; //!< An iterator over GPUInfos
/** A const iterator over GPUInfos. */
typedef GPUInfos::const_iterator GPUInfosCIter;

struct NetInfo;
typedef std::vector<NetInfo> NetInfos;
typedef NetInfos::iterator NetInfosIter;
typedef NetInfos::const_iterator NetInfosCIter;

class Filter;
typedef lunchbox::RefPtr<Filter> FilterPtr; //!< A shared Filter pointer

typedef Module<GPUInfos> GPUModule;
typedef Module<NetInfos> NetModule;
}

#endif // HWSD_TYPES_H
