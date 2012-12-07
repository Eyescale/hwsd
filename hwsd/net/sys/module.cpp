
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

#ifdef __linux
#  include <arpa/inet.h>
#  include <netdb.h>
#  include <net/if.h>
#  include <string.h>
#  include <sys/ioctl.h>
#  include <linux/ethtool.h>
#  include <linux/sockios.h>
#else
#  include <lunchbox/debug.h>
#endif


namespace hwsd
{
namespace net
{
namespace sys
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

bool Module::announce() const
{
    const NetInfos& nets = discover();
    for( hwsd::NetInfosCIter i = nets.begin(); i != nets.end(); ++i )
    {
        const NetInfo& info = *i;
        std::cout << info << std::endl;
    }
    return true;
}

NetInfos Module::discover() const
{
    NetInfos result;

#ifdef _WIN32
    LBUNIMPLEMENTED
#elif defined __APPLE__
    LBUNIMPLEMENTED
#else
    int socketfd = socket( PF_INET, SOCK_DGRAM, 0 );
    if( socketfd < 0 )
        return result;

    char data[8192];
    ifconf ifc;
    ifc.ifc_len = sizeof(data);
    ifc.ifc_buf = data;
    if( ioctl( socketfd, SIOCGIFCONF, &ifc ) < 0 )
    {
        close( socketfd );
        return result;
    }

    char address[INET6_ADDRSTRLEN];
    char hostname[NI_MAXHOST];

    ifreq* ifr = ifc.ifc_req;
    const size_t nInterfaces = ifc.ifc_len / sizeof(ifreq);
    for( size_t i = 0; i < nInterfaces; ++i )
    {
        ifreq* item = &ifr[i];

        NetInfo info;
        info.type = NetInfo::TYPE_ETHERNET;
        info.name = item->ifr_name;

        switch( item->ifr_addr.sa_family )
        {
        case AF_INET:
            info.inetAddress = inet_ntop( item->ifr_addr.sa_family,
                                     &((sockaddr_in*)&item->ifr_addr)->sin_addr,
                                                      address, sizeof(address));
            if( getnameinfo( &item->ifr_addr, sizeof(sockaddr_in), hostname,
                             sizeof(hostname), 0, 0, NI_NAMEREQD ) == 0 )
            {
                info.hostname = hostname;
            }
            break;

        case AF_INET6:
            info.inet6Address = inet_ntop( item->ifr_addr.sa_family,
                                   &((sockaddr_in6*)&item->ifr_addr)->sin6_addr,
                                                      address, sizeof(address));
            if( getnameinfo( &item->ifr_addr, sizeof(sockaddr_in6), hostname,
                             sizeof(hostname), 0, 0, NI_NAMEREQD ) == 0 )
            {
                info.hostname = hostname;
            }
            break;
        }

        if( ioctl( socketfd, SIOCGIFHWADDR, item ) >= 0 )
        {
            std::ostringstream mac;
            mac << std::setfill( '0' ) << std::hex;
            for( size_t j = 0; j < 5; ++j )
                mac << std::setw( 2 ) << int(item->ifr_hwaddr.sa_data[j]) << ":";
            mac << std::setw( 2 ) << int(item->ifr_hwaddr.sa_data[5]);
            info.hwAddress = mac.str();
        }

        if( ioctl( socketfd, SIOCGIFFLAGS, item ) >= 0 )
            info.up = item->ifr_flags & IFF_UP;

        ethtool_cmd edata;
        item->ifr_data = (caddr_t)&edata;
        edata.cmd = ETHTOOL_GSET;
        if( ioctl( socketfd, SIOCETHTOOL, item ) >= 0 )
            info.linkspeed = edata.speed;

        result.push_back( info );
    }

    close( socketfd );
#endif

    return result;
}

}
}
}
