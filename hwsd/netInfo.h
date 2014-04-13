
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

#ifndef HWSD_NETINFO_H
#define HWSD_NETINFO_H

#include <hwsd/api.h>
#include <hwsd/nodeInfo.h>  // base class

#include <lunchbox/debug.h>
#include <climits>
#include <ostream>

namespace hwsd
{
/** A structure containing network-specific information. */
struct NetInfo : public NodeInfo
{
    enum Type
    {
        TYPE_ETHERNET = LB_BIT1,
        TYPE_INFINIBAND = LB_BIT2,
        TYPE_LOOPBACK = LB_BIT3,
        TYPE_TUNNEL_ETHERNET = LB_BIT4, //!< gif interface on Darwin
        TYPE_TUNNEL_6TO4 = LB_BIT5, //!< stf interface on Darwin
        TYPE_UNKNOWN = LB_BIT31,
        TYPE_ALL = LB_BIT_ALL_32
    };

    /** A non-enumerated value. @version 1.0 */
    static const unsigned defaultValue = UINT_MAX;

    /** Default constructor pointing to the default display. @version 1.0 */
    HWSD_API NetInfo();

    /** @return true if both informations are identical. @version 1.0 */
    HWSD_API bool operator == ( const NetInfo& rhs ) const;

    /** @return true if both infos are not identical. @version 1.0 */
    HWSD_API bool operator != ( const NetInfo& rhs ) const;

    HWSD_API std::string getType() const;

    HWSD_API void setType( const std::string& strType );

    /** The type of the network interface. @version 1.0 */
    Type type;

    /** The name of the interface (e.g. eth0, ib0). @version 1.0 */
    std::string name;

    /** The hostname assigned to this interface. @version 1.0 */
    std::string hostname;

    /** The MAC address (':' as separator) of the interface. @version 1.0 */
    std::string hwAddress;

    /** The IPv4 address (':' as separator) of the interface. @version 1.0*/
    std::string inetAddress;

    /** The IPv6 address (':' as separator) of the interface. @version 1.0*/
    std::string inet6Address;

    /** The interface link speed in Megabits per second. @version 1.0 */
    unsigned int linkspeed;

    /** Whether the interface is up or down. @version 1.0 */
    bool up;
};

inline std::ostream& operator << ( std::ostream& os, const NetInfo& info )
{
    os << "NetInfo\n" << static_cast< const NodeInfo& >( info );
        os << "  Type      " << info.getType() << std::endl;
        os << "  Status    " << (info.up ? "UP" : "DOWN") << std::endl;
    if( !info.name.empty( ))
        os << "  Name      " << info.name << std::endl;
    if( !info.hostname.empty( ))
        os << "  Hostname  " << info.hostname << std::endl;
    if( !info.hwAddress.empty( ))
        os << "  HWaddr    " << info.hwAddress << std::endl;
    if( !info.inetAddress.empty( ))
        os << "  IPv4      " << info.inetAddress << std::endl;
    if( !info.inet6Address.empty( ))
        os << "  IPv6      " << info.inet6Address << std::endl;
    if( info.linkspeed != NetInfo::defaultValue )
        os << "  Linkspeed " << info.linkspeed << "Mbps" << std::endl;

    return os;
}
}

#endif // HWSD_NETINFO_H
