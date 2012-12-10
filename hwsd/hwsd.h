
/* Copyright (c) 2012, Daniel Nachbaur <danielnachbaur@gmail.com>
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

#ifndef HWSD_HWSD_H
#define HWSD_HWSD_H

#include <hwsd/api.h>
#include <hwsd/filter.h>
#include <hwsd/types.h>


namespace hwsd
{
    /**
     * Announce the information about all found network GPUs.
     *
     * @return true if announcement was successful
     * @version 1.0
     */
    HWSD_API bool announceGPUInfos( const std::string& session );

    /** @return information about all found GPUs. @version 1.0 */
    HWSD_API GPUInfos discoverGPUInfos( FilterPtr filter =
                                            FilterPtr( new DuplicateFilter ));

    /**
     * Announce the information about all found network interfaces.
     *
     * @return true if announcement was successful
     * @version 1.0
     */
    HWSD_API bool announceNetInfos( const std::string& session );

    /** @return information about all found network interfaces. @version 1.0 */
    HWSD_API NetInfos discoverNetInfos();

    /** @return the local node identifier. @version 1.0 */
    HWSD_API const lunchbox::UUID& getLocalNodeID();
}

#endif // HWSD_HWSD_H
