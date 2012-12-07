
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
std::string NETCOUNT     = "Net Count";

std::string NETTYPE      = "Type";
std::string NETNAME      = "Name";
std::string NETHOSTNAME  = "Hostname";
std::string NETHWADDR    = "HWaddr";
std::string NETINETADDR  = "inet addr";
std::string NETINET6ADDR = "inet6 addr";
std::string NETLINKSPEED = "linkspeed";
std::string NETUP        = "up";
}

namespace detail
{
class Module
{
public:
    Module()
        : service( NETSERVICE )
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
    void setValue( const size_t index, const std::string& key, const T& value )
    {
        std::ostringstream out;
        out << NET << index << " " << key;
        const std::string string = out.str();

        out.str("");
        out << value;
        service.set( string, out.str( ));
    }

    lunchbox::Servus service;
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

bool Module::announce() const
{
    const NetInfos& nets = hwsd::discoverNets();
    if( nets.empty( ))
        return true;

    std::ostringstream out;
    out << nets.size();
    _impl->service.set( NETCOUNT, out.str( ));
    for( hwsd::NetInfosCIter i = nets.begin(); i != nets.end(); ++i )
    {
        const NetInfo& info = *i;
        const size_t index = i - nets.begin();

        _impl->setValue( index, NETTYPE, info.getType( ));
        _impl->setValue( index, NETNAME, info.name );
        _impl->setValue( index, NETHOSTNAME, info.hostname );
        _impl->setValue( index, NETHWADDR, info.hwAddress );
        _impl->setValue( index, NETINETADDR, info.inetAddress );
        _impl->setValue( index, NETINET6ADDR, info.inet6Address );
        _impl->setValue( index, NETLINKSPEED, info.linkspeed );
        _impl->setValue( index, NETUP, info.up );
    }

    return _impl->service.announce( 4242, "" );
}

NetInfos Module::discover() const
{
    NetInfos infos[2];
    lunchbox::Servus::Interface interfaces[2] = { lunchbox::Servus::IF_ALL,
                                                  lunchbox::Servus::IF_LOCAL };
    for( unsigned i = 0; i < 2; ++i )
    {
        const lunchbox::Strings& hosts = _impl->service.discover( interfaces[i],
                                                                  WAIT_TIME );
        for( lunchbox::StringsCIter j = hosts.begin(); j != hosts.end(); ++j )
        {
            const std::string& host = *j;
            unsigned nNets = 0;

            const std::string& data = _impl->service.get( host, NETCOUNT );
            if( data.empty( ))
                continue;
            std::istringstream in( data );
            in >> nNets;
            for( unsigned k = 0; k < nNets; ++k )
            {
                NetInfo info;
                std::string type;
                _impl->getValue( host, k, NETTYPE, type );
                info.setType( type );
                _impl->getValue( host, k, NETNAME, info.name );
                _impl->getValue( host, k, NETHOSTNAME, info.hostname );
                _impl->getValue( host, k, NETHWADDR, info.hwAddress );
                _impl->getValue( host, k, NETINETADDR, info.inetAddress );
                _impl->getValue( host, k, NETINET6ADDR, info.inet6Address );
                _impl->getValue( host, k, NETLINKSPEED, info.linkspeed );
                _impl->getValue( host, k, NETUP, info.up );
                infos[i].push_back( info );
            }
        }
    }

    return infos[0];
}

}
}
}
