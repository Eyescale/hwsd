
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

#include <hwsd/hwsd.h>
#include <hwsd/netInfo.h>
#include <lunchbox/servus.h>
#include <algorithm>
#include <sstream>

#define WAIT_TIME 500 // ms

namespace hwsd
{
namespace net
{
namespace dns_sd
{
namespace
{
static Module* instance = 0;

std::string NETSERVICE   = "_net-sd._tcp";
std::string NET          = "Net";
std::string NETCOUNT     = "Count";
std::string NETSESSION   = "Session";
std::string NETNODEID    = "NodeID";

std::string NETTYPE      = "Type";
std::string NETNAME      = "Name";
std::string NETHOSTNAME  = "Hostname";
std::string NETHWADDR    = "MAC";
std::string NETINETADDR  = "IPv4";
std::string NETINET6ADDR = "IPv6";
std::string NETLINKSPEED = "Linkspeed";
std::string NETUP        = "Up";
}

namespace detail
{
class Module
{
public:
    Module()
        : service( NETSERVICE )
        , announcing( false )
    {}

    template< class T >
    bool getValue( const std::string& host, const size_t index,
                   const std::string& key, T& value )
    {
        std::ostringstream out;
        out << NET << index << " " << key;
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
        out << NET << " " << key;
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
        out << NET << index << " " << key;
        const std::string string = out.str();

        out.str("");
        out << value;
        service.set( string, out.str( ));
    }

    template< class T >
    void setValue( const std::string& key, const T& value )
    {
        std::ostringstream out;
        out << NET << " " << key;
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
    : NetModule()
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

bool Module::announce( const lunchbox::UUID& nodeID,
                       const std::string& session ) const
{
    _impl->announcing = true;
    NetInfos nets = hwsd::discoverNetInfos();
    _impl->announcing = false;
    if( nets.empty( ))
        return true;

    for( hwsd::NetInfosIter i = nets.begin(); i != nets.end(); ++i )
    {
        if( (*i).type == NetInfo::TYPE_LOOPBACK )
        {
            nets.erase( i );
            break;
        }
    }

    _impl->setValue( NETCOUNT, nets.size( ));
    _impl->setValue( NETSESSION, session );
    _impl->setValue( NETNODEID, nodeID );

    for( hwsd::NetInfosCIter i = nets.begin(); i != nets.end(); ++i )
    {
        const NetInfo& info = *i;
        const size_t index = i - nets.begin();

        _impl->setValue( index, NETTYPE, info.getType( ));

        if( !info.name.empty( ))
            _impl->setValue( index, NETNAME, info.name );

        if( !info.hostname.empty( ))
            _impl->setValue( index, NETHOSTNAME, info.hostname );

        if( !info.hwAddress.empty( ))
            _impl->setValue( index, NETHWADDR, info.hwAddress );

        if( !info.inetAddress.empty( ))
            _impl->setValue( index, NETINETADDR, info.inetAddress );

        if( !info.inet6Address.empty( ))
            _impl->setValue( index, NETINET6ADDR, info.inet6Address );

        if( info.linkspeed != NetInfo::defaultValue )
            _impl->setValue( index, NETLINKSPEED, info.linkspeed );

        _impl->setValue( index, NETUP, info.up );
    }

    return _impl->service.announce( 4242, "" );
}

NetInfos Module::discover() const
{
    NetInfos infos;
    if( _impl->announcing )
        return infos;

    const lunchbox::Strings& hosts =
        _impl->service.discover( lunchbox::Servus::IF_ALL, WAIT_TIME );

    for( lunchbox::StringsCIter i = hosts.begin(); i != hosts.end(); ++i )
    {
        const std::string& host = *i;

        unsigned nNets = 0;
        _impl->getValue( host, NETCOUNT, nNets );

        for( unsigned j = 0; j < nNets; ++j )
        {
            NetInfo info;
            _impl->getValue( host, NETSESSION, info.session );
            if( !_impl->getValue( host, NETNODEID, info.id ))
                info.id = lunchbox::make_uint128( host.c_str( ));

            std::string type;
            _impl->getValue( host, j, NETTYPE, type );
            info.setType( type );
            _impl->getValue( host, j, NETNAME, info.name );
            _impl->getValue( host, j, NETHOSTNAME, info.hostname );
            _impl->getValue( host, j, NETHWADDR, info.hwAddress );
            _impl->getValue( host, j, NETINETADDR, info.inetAddress );
            _impl->getValue( host, j, NETINET6ADDR, info.inet6Address );
            _impl->getValue( host, j, NETLINKSPEED, info.linkspeed );
            _impl->getValue( host, j, NETUP, info.up );

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
