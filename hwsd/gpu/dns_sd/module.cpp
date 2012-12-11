
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
#include <hwsd/hwsd.h>
#include <lunchbox/servus.h>
#include <algorithm>
#include <sstream>

#define WAIT_TIME 500 // ms

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
std::string GPUSESSION  = "Session";
std::string GPUNODEID   = "NodeID";

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
class Module
{
public:
    Module()
        : service( GPUSERVICE )
        , announcing( false )
    {}

    template< class T >
    bool getValue( const std::string& host, const size_t index,
                   const std::string& key, T& value )
    {
        std::ostringstream out;
        out << GPU << index << " " << key;
        const std::string string = out.str();

        const std::string& data = service.get( host, string );
        if( data.empty( ))
            return false;

        std::istringstream in( data );
        in >> value;
        return true;
    }

    template< class T >
    bool getValue( const std::string& host, const std::string& key, T& value )
    {
        std::ostringstream out;
        out << GPU << " " << key;
        const std::string string = out.str();

        const std::string& data = service.get( host, string );
        if( data.empty( ))
            return false;

        std::istringstream in( data );
        in >> value;
        return true;
    }

    template< class T >
    void setValue( const size_t index, const std::string& key, const T& value )
    {
        std::ostringstream out;
        out << GPU << index << " " << key;
        const std::string string = out.str();

        out.str("");
        out << value;
        service.set( string, out.str( ));
    }

    template< class T >
    void setValue( const std::string& key, const T& value )
    {
        std::ostringstream out;
        out << GPU << " " << key;
        const std::string string = out.str();

        out.str("");
        out << value;
        service.set( string, out.str( ));
    }

    lunchbox::Servus service;
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

    _impl->setValue( GPUCOUNT, gpus.size( ));
    _impl->setValue( GPUSESSION, session );
    _impl->setValue( GPUNODEID, getLocalNodeID( ));

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
    GPUInfos infos;
    if( _impl->announcing )
        return infos;

    const lunchbox::Strings& hosts =
        _impl->service.discover( lunchbox::Servus::IF_ALL, WAIT_TIME );

    for( lunchbox::StringsCIter i = hosts.begin(); i != hosts.end(); ++i )
    {
        const std::string& host = *i;

        unsigned nGPUs = 0;
        _impl->getValue( host, GPUCOUNT, nGPUs );
        for( unsigned j = 0; j < nGPUs; ++j )
        {
            std::string type;
            if( !_impl->getValue( host, j, GPUTYPE, type ))
                continue;

            GPUInfo info( type );
            _impl->getValue( host, GPUSESSION, info.session );
            if( !_impl->getValue( host, GPUNODEID, info.id ))
                info.id = lunchbox::make_uint128( host.c_str( ));

            _impl->getValue( host, j, GPUPORT, info.port );
            _impl->getValue( host, j, GPUDEVICE, info.device );
            _impl->getValue( host, j, GPUX, info.pvp[0] );
            _impl->getValue( host, j, GPUY, info.pvp[1] );
            _impl->getValue( host, j, GPUWIDTH, info.pvp[2] );
            _impl->getValue( host, j, GPUHEIGHT, info.pvp[3] );
            _impl->getValue( host, j, GPUFLAGS, info.flags );

            if( info.id != getLocalNodeID( ))
                info.nodeName = host;

            infos.push_back( info );
        }
    }
    return infos;
}

}
}
}
