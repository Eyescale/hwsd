
/* Copyright (c) 2012, Daniel Nachbaur <danielnachbaur@gmail.com>
 *               2013, Stefan.Eilemann@epfl.ch
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

//#define USE_IOCTL

#ifdef _WIN32
#  include <winsock2.h>
#  include <Ws2tcpip.h>
#  include <iphlpapi.h>
#elif defined USE_IOCTL
#  include <arpa/inet.h>
#  include <netdb.h>
#  include <string.h>
#  include <sys/ioctl.h>
#  include <unistd.h>
#  include <linux/ethtool.h>
#  include <linux/sockios.h>
#else
#  include <arpa/inet.h>
#  include <netdb.h>
#  include <ifaddrs.h>
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <map>
#  ifdef __linux__
#    include <linux/if_arp.h>
#  endif
#  ifdef __APPLE__
#    include <net/if.h>
#    include <net/if_dl.h>
#    include <net/if_types.h>
#    define AF_PACKET AF_LINK
#  endif
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

NetInfos Module::discover() const
{
    NetInfos result;
#ifdef _WIN32
    WSAData d;
    if( WSAStartup( MAKEWORD(2, 0), &d ) != 0 )
        return result;

    DWORD datasize = 0;
    if( GetAdaptersAddresses( AF_INET, GAA_FLAG_INCLUDE_PREFIX, 0,
                              0, &datasize ) != ERROR_BUFFER_OVERFLOW )
    {
        return result;
    }

    PIP_ADAPTER_ADDRESSES addresses = (PIP_ADAPTER_ADDRESSES)malloc( datasize );
    if( GetAdaptersAddresses( AF_INET, GAA_FLAG_INCLUDE_PREFIX, 0,
                              addresses, &datasize ) != ERROR_SUCCESS )
    {
        free( addresses );
        return result;
    }

    char buf[BUFSIZ];
    for( PIP_ADAPTER_ADDRESSES current = addresses; current;
         current = current->Next )
    {
        NetInfo info;

        switch( current->IfType )
        {
        case IF_TYPE_ETHERNET_CSMACD:
            info.type = NetInfo::TYPE_ETHERNET;
            break;
        case IF_TYPE_SOFTWARE_LOOPBACK:
            info.type = NetInfo::TYPE_LOOPBACK;
            break;
        }

        info.up = current->OperStatus == IfOperStatusUp;
        info.linkspeed = current->TransmitLinkSpeed / 1000000;

        memset( buf, 0, BUFSIZ );
        WideCharToMultiByte( CP_ACP, 0, current->FriendlyName,
                             int( wcslen( current->FriendlyName )), buf, BUFSIZ,
                             0, 0 );
        info.name = buf;

        for( PIP_ADAPTER_UNICAST_ADDRESS addr = current->FirstUnicastAddress;
             addr; addr = addr->Next )
        {
            memset( buf, 0, BUFSIZ );
            getnameinfo( addr->Address.lpSockaddr,
                         addr->Address.iSockaddrLength, buf, sizeof(buf), 0, 0,
                         NI_NUMERICHOST );
            if( addr->Address.lpSockaddr->sa_family == AF_INET )
                info.inetAddress = buf;
            else
                info.inet6Address = buf;
        }

        if( current->PhysicalAddressLength != 0 )
        {
            std::ostringstream mac;
            mac << std::setfill( '0' ) << std::hex;
            size_t i = 0;
            for( ; i < current->PhysicalAddressLength-1; ++i )
                mac << std::setw( 2 ) << int(current->PhysicalAddress[i]) <<":";
            mac << std::setw( 2 ) << int(current->PhysicalAddress[i]);
            info.hwAddress = mac.str();
        }

        result.push_back( info );
    }

    free( addresses );
    WSACleanup();

#else // _WIN32

#  ifdef USE_IOCTL
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
        {
            info.up = item->ifr_flags & IFF_UP;
            if( item->ifr_flags & IFF_LOOPBACK )
                info.type = NetInfo::TYPE_LOOPBACK;
        }

        ethtool_cmd edata;
        item->ifr_data = (caddr_t)&edata;
        edata.cmd = ETHTOOL_GSET;
        if( ioctl( socketfd, SIOCETHTOOL, item ) >= 0 )
            info.linkspeed = edata.speed;

        result.push_back( info );
    }

    close( socketfd );

#  else // USE_IOCTL

    ifaddrs* ifaddr;
    if( getifaddrs( &ifaddr ) == -1 )
        return result;

    typedef std::map< std::string, NetInfo > Name2Info;
    Name2Info infos;

    char host[NI_MAXHOST];
    for( ifaddrs* ifa = ifaddr; ifa; ifa = ifa->ifa_next )
    {
        if( !ifa->ifa_addr )
           continue;

        NetInfo& info = infos[ifa->ifa_name];
        info.name = ifa->ifa_name;
        info.up = ifa->ifa_flags & IFF_UP;

        const int family = ifa->ifa_addr->sa_family;

        switch( family )
        {
          case AF_INET:
              if( getnameinfo( ifa->ifa_addr, sizeof(sockaddr_in), host,
                               NI_MAXHOST, 0, 0, NI_NUMERICHOST ) == 0 )
              {
                  info.inetAddress = host;
              }
              if( getnameinfo( ifa->ifa_addr, sizeof(sockaddr_in), host,
                               NI_MAXHOST, 0, 0, NI_NAMEREQD ) == 0 )
              {
                  info.hostname = host;
              }
              break;

          case AF_INET6:
              if( getnameinfo( ifa->ifa_addr, sizeof(sockaddr_in6), host,
                               NI_MAXHOST, 0, 0, NI_NUMERICHOST ) == 0 )
              {
                  info.inet6Address = host;
              }
              break;

          case AF_PACKET:
          {
              std::ostringstream mac;
              mac << std::setfill( '0' ) << std::hex;
#ifdef __APPLE__
              const sockaddr_dl* link = (sockaddr_dl*)ifa->ifa_addr;
              const uint8_t* macaddr = (uint8_t*)LLADDR(link);

              for( size_t i = 0; i < 6; ++i )
              {
                  mac << std::setw( 2 ) << int( macaddr[i] )
                      << (i < 5 ? ":" : "");
              }

              switch( link->sdl_type )
              {
                case IFT_ETHER:
                    info.type = NetInfo::TYPE_ETHERNET;
                    break;
                case IFT_LOOP:
                    info.type = NetInfo::TYPE_LOOPBACK;
                    break;
                case IFT_GIF:
                    info.type = NetInfo::TYPE_TUNNEL_ETHERNET;
                    break;
                case IFT_STF:
                    info.type = NetInfo::TYPE_TUNNEL_6TO4;
                    break;

                default:
                    LBWARN << "Unhandled interface type: 0x" << std::hex
                           << unsigned( link->sdl_type ) << std::dec
                           << std::endl;
                    info.type = NetInfo::TYPE_UNKNOWN;
                    break;
              }
#else // __APPLE__
              sockaddr_ll* s = (sockaddr_ll*)ifa->ifa_addr;
              for( size_t i = 0; i < 6; ++i )
              {
                  mac << std::setw( 2 ) << int(s->sll_addr[i])
                      << (i < 5 ? ":" : "");
              }

              switch( s->sll_hatype )
              {
                case ARPHRD_ETHER:
                    info.type = NetInfo::TYPE_ETHERNET;
                    break;
                case ARPHRD_LOOPBACK:
                    info.type = NetInfo::TYPE_LOOPBACK;
                    break;
                case ARPHRD_INFINIBAND:
                    info.type = NetInfo::TYPE_INFINIBAND;
                    break;

                default:
                    LBWARN << "Unhandled interface type: " << s->sll_hatype
                           << std::endl;
                    info.type = NetInfo::TYPE_UNKNOWN;
              }
#endif // !__APPLE__
              info.hwAddress = mac.str();
              break;
          }

          default:
              LBWARN << "Unhandled protocol type: " << family << std::endl;
        }
    }

    freeifaddrs( ifaddr );

    for( Name2Info::const_iterator it = infos.begin(); it != infos.end(); ++it )
        result.push_back( it->second );
#  endif // !USE_IOCTL
#endif // !_WIN32
    return result;
}

}
}
}
