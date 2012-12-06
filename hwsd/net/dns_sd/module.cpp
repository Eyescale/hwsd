
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

template< class T >
bool getValue( const lunchbox::Servus& service, const std::string& host,
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

void Module::dispose()
{
    delete instance;
    instance = 0;
}

NetInfos Module::discover() const
{
    lunchbox::Servus service( "_net-sd._tcp" );

    NetInfos infos;
    lunchbox::Servus::Interface interfaces[2] = { lunchbox::Servus::IF_ALL,
                                                  lunchbox::Servus::IF_LOCAL };
    for( unsigned i = 0; i < 2; ++i )
    {
        const lunchbox::Strings& hosts = service.discover( interfaces[i],
                                                           WAIT_TIME );
        for( lunchbox::StringsCIter j = hosts.begin(); j != hosts.end(); ++j )
        {

        }
    }

    return infos;
}

}
}
}
