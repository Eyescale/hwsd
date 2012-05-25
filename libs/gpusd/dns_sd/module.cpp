
/* Copyright (c) 2011-2012, Stefan Eilemann <eile@eyescale.ch> 
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

#include <gpusd/gpuInfo.h>
#include <servus/servus.h>
#include <algorithm>
#include <sstream>

#define WAIT_TIME 500 // ms

namespace gpusd
{
namespace dns_sd
{
namespace
{
Module* instance = 0;

template< class T >
bool getValue( const servus::Service& service, const std::string& host,
               const std::string& key, T& value)
{
    const std::string& data = service.get( host, key );
    if( data.empty( ))
        return false;

    std::istringstream in( data );
    in >> value;
    return true;
}
}

void Module::use()
{
    if( !instance )
        instance = new Module;
}

GPUInfos Module::discoverGPUs_() const
{
    servus::Service service( "_gpu-sd._tcp" );

    GPUInfos infos[2];
    servus::Interface interfaces[2] = { servus::IF_ALL, servus::IF_LOCAL };
    for( unsigned i = 0; i < 2; ++i )
    {
        const servus::Strings& hosts = service.discover( interfaces[i], 500 );
        for( servus::StringsCIter j = hosts.begin(); j != hosts.end(); ++j )
        {
            const std::string& host = *j;
            unsigned nGPUs = 0;
            getValue( service, host, "GPU Count", nGPUs );
            for( unsigned k = 0; k < nGPUs; ++k )
            {
                std::ostringstream out;
                out << "GPU" << k << " ";
                const std::string& gpu = out.str();

                std::string type;
                if( !getValue( service, host, gpu + "Type", type ))
                    continue;

                GPUInfo info( type );
                info.hostname = host;
                getValue( service, host, "Session", info.session );
                getValue( service, host, "Hostname", info.hostname );
                getValue( service, host, gpu + "Port", info.port );
                getValue( service, host, gpu + "Device", info.device );
                getValue( service, host, gpu + "X", info.pvp[0] );
                getValue( service, host, gpu + "Y", info.pvp[1] );
                getValue( service, host, gpu + "Width", info.pvp[2] );
                getValue( service, host, gpu + "Height", info.pvp[3] );
                getValue( service, host, gpu + "Flags", info.flags );
                infos[i].push_back( info );
            }
        }
    }

    // set localhost records to localhost
    const GPUInfosIter localEnd = infos[1].end();
    for( GPUInfosIter i = infos[0].begin(); i != infos[0].end(); ++i )
    {
        GPUInfo& info = *i;
        if( std::find( infos[1].begin(), localEnd, info ) != localEnd )
            info.hostname.clear();
    }
    return infos[0];
}

}
}
