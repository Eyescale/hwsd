
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
    /** @return information about all found GPUs. @version 1.0 */
    HWSD_API GPUInfos discoverGPUs( FilterPtr filter =
                                              FilterPtr( new DuplicateFilter ));

    HWSD_API bool announceNets();

    /** @return information about all found network interfaces. @version 1.0 */
    HWSD_API NetInfos discoverNets();
}

#endif // HWSD_HWSD_H
