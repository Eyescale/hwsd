
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

#ifdef _WIN32
#  include <winsock2.h>
#  include <Ws2tcpip.h>
#  include <iphlpapi.h>
#elif defined __linux
#  include <arpa/inet.h>
#  include <netdb.h>
#  include <net/if.h>
#  include <string.h>
#  include <sys/ioctl.h>
#  include <unistd.h>
#  include <linux/ethtool.h>
#  include <linux/sockios.h>
#elif defined __APPLE__
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

NetInfos Module::discover() const
{
#ifdef _WIN32
    return _discoverWin32();
#elif defined __APPLE__
    return _discoverMac();
#else
    return _discoverLinux();
#endif
}

#ifdef _WIN32
NetInfos Module::_discoverWin32() const
{
    NetInfos result;
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
                             wcslen(current->FriendlyName), buf, BUFSIZ, 0, 0 );
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

    return result;
}

#elif defined __linux

NetInfos Module::_discoverLinux() const
{
    NetInfos result;

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
    return result;
}

#elif defined __APPLE__

NetInfos Module::_discoverMac() const
{
    NetInfos result;
    LBUNIMPLEMENTED
    return result;
}

#endif

}
}
}
