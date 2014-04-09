
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

#include <hwsd/gpuInfo.h>
#include <hwsd/functions.h>
#include <hwsd/detail/dns_sd_module.h>

#include <algorithm>
#include <sstream>

#define WAIT_TIME 50 // ms after last update

namespace hwsd
{
namespace gpu
{
namespace dns_sd
{
namespace
{
static Module* instance = 0;

std::string GPUSERVICE  = "_gpu-sd._tcp";
std::string GPU         = "GPU";
std::string GPUCOUNT    = "Count";

std::string GPUTYPE     = "Type";
std::string GPUPORT     = "Port";
std::string GPUDEVICE   = "Device";
std::string GPUX        = "X";
std::string GPUY        = "Y";
std::string GPUWIDTH    = "Width";
std::string GPUHEIGHT   = "Height";
std::string GPUFLAGS    = "Flags";
}

namespace detail
{
class Module : public hwsd::detail::dns_sd::Module
{
public:
    Module()
        : hwsd::detail::dns_sd::Module( GPUSERVICE, GPU )
        , announcing( false )
    {}

    bool announcing;
};
}

Module::Module()
    : GPUModule()
    , _impl( new detail::Module )
{}

Module::~Module()
{
    delete _impl;
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

bool Module::announce( const std::string& session ) const
{
    _impl->announcing = true;
    const GPUInfos& gpus = hwsd::discoverGPUInfos();
    _impl->announcing = false;
    if( gpus.empty( ))
        return true;

    NodeInfo nodeInfo;
    nodeInfo.session = session;
    _impl->announce( nodeInfo );

    _impl->setValue( GPUCOUNT, gpus.size( ));
    for( hwsd::GPUInfosCIter i = gpus.begin(); i != gpus.end(); ++i )
    {
        const GPUInfo& info = *i;
        const size_t index = i - gpus.begin();

        // GPU<integer> Type=GLX | WGL | WGLn | CGL
        _impl->setValue( index, GPUTYPE, info.getName( ));

        if( info.port != GPUInfo::defaultValue )
            // GPU<integer> Port=<integer> // X11 display number, 0 otherwise
            _impl->setValue( index, GPUPORT, info.port );

        if( info.device != GPUInfo::defaultValue )
            // GPU<integer> Device=<integer> // X11 display number, 0 otherwise
            _impl->setValue( index, GPUDEVICE, info.device );

        if( info.pvp[2] > 0 && info.pvp[3] > 0 )
        {
            _impl->setValue( index, GPUX, info.pvp[0] );
            _impl->setValue( index, GPUY, info.pvp[1] );
            _impl->setValue( index, GPUWIDTH, info.pvp[2] );
            _impl->setValue( index, GPUHEIGHT, info.pvp[3] );
        }

        if( info.flags != 0 )
            _impl->setValue( index, GPUFLAGS, info.flags );
    }

    return _impl->service.announce( 4242, "" );
}

GPUInfos Module::discover() const
{
    GPUInfos infos[2];
    if( _impl->announcing )
        return infos[0];

    lunchbox::Servus::Interface interfaces[2] = { lunchbox::Servus::IF_ALL,
                                                  lunchbox::Servus::IF_LOCAL };

    for( unsigned i = 0; i < 2; ++i )
    {
        const lunchbox::Strings& hosts = _impl->service.discover( interfaces[i],
                                                                  WAIT_TIME );
        for( lunchbox::StringsCIter j = hosts.begin(); j != hosts.end(); ++j )
        {
            const std::string& host = *j;

            unsigned nGPUs = 0;
            _impl->getValue( host, GPUCOUNT, nGPUs );
            for( unsigned k = 0; k < nGPUs; ++k )
            {
                std::string type;
                if( !_impl->getValue( host, k, GPUTYPE, type ))
                    continue;

                GPUInfo info( type );
                _impl->discover( host, info );

                _impl->getValue( host, k, GPUPORT, info.port );
                _impl->getValue( host, k, GPUDEVICE, info.device );
                _impl->getValue( host, k, GPUX, info.pvp[0] );
                _impl->getValue( host, k, GPUY, info.pvp[1] );
                _impl->getValue( host, k, GPUWIDTH, info.pvp[2] );
                _impl->getValue( host, k, GPUHEIGHT, info.pvp[3] );
                _impl->getValue( host, k, GPUFLAGS, info.flags );

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
            info.nodeName.clear();
    }
    return infos[0];
}

}
}
}
