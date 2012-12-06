
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

#include "module.h"

#include <hwsd/netInfo.h>

#include <lunchbox/log.h>

#include <hwloc.h>


namespace hwsd
{
namespace net
{
namespace hwloc
{
namespace
{
static Module* instance = 0;
}

void Module::use()
{
    if( !instance )
        instance = new Module;
}

void Module::dispose()
{
    delete instance;
    instance = 0;
}

NetInfos Module::discover() const
{
    NetInfos result;

    hwloc_topology_t topology;
    hwloc_topology_init( &topology );

    if( hwloc_topology_set_flags( topology, HWLOC_TOPOLOGY_FLAG_IO_DEVICES ) < 0 )
    {
        LBERROR << "hwloc_topology_set_flags failed" << std::endl;
        hwloc_topology_destroy( topology );
        return result;
    }

    if( hwloc_topology_load( topology ) < 0 )
    {
        LBERROR << "hwloc_topology_load failed" << std::endl;
        hwloc_topology_destroy( topology );
        return result;
    }

    hwloc_obj_t obj = hwloc_get_next_pcidev( topology, 0 );
    for(  ; obj; obj = hwloc_get_next_pcidev( topology, obj ))
    {
        LBINFO << obj->name << std::endl;
    }

    hwloc_topology_destroy( topology );
    return result;
}

}
}
}
