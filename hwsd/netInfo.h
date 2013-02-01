
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
            TYPE_UNKNOWN = LB_BIT_NONE,
            TYPE_ETHERNET = LB_BIT1,
            TYPE_INFINIBAND = LB_BIT2,
            TYPE_LOOPBACK = LB_BIT3,
            TYPE_TUNNEL_ETHERNET = LB_BIT4, //!< gif interface on Darwin
            TYPE_TUNNEL_6TO4 = LB_BIT5, //!< stf interface on Darwin
            TYPE_ALL = LB_BIT_ALL_32
        };

        /** A non-enumerated value. @version 1.0 */
        static const unsigned defaultValue = UINT_MAX;

        /** Default constructor pointing to the default display. @version 1.0 */
        NetInfo() : type( TYPE_UNKNOWN ), linkspeed( defaultValue )
                  , up( false ) {}

        /** @return true if both informations are identical. @version 1.0 */
        bool operator == ( const NetInfo& rhs ) const
            {
                return NodeInfo::operator==( rhs ) && type == rhs.type &&
                       name == rhs.name && hostname == rhs.hostname &&
                       hwAddress == rhs.hwAddress &&
                       inetAddress == rhs.inetAddress &&
                       inet6Address == rhs.inet6Address &&
                       linkspeed == rhs.linkspeed && up == rhs.up;
            }

        /** @return true if both infos are not identical. @version 1.0 */
        bool operator != ( const NetInfo& rhs ) const
            {
                return !(*this == rhs );
            }

        std::string getType() const
        {
            switch( type )
            {
            case NetInfo::TYPE_ETHERNET:
                return "ETHERNET";
            case NetInfo::TYPE_INFINIBAND:
                return "INFINIBAND";
            case NetInfo::TYPE_LOOPBACK:
                return "LOOPBACK";
            case NetInfo::TYPE_TUNNEL_ETHERNET:
                return "TUNNEL_ETHERNET";
            case NetInfo::TYPE_TUNNEL_6TO4:
                return "TUNNEL_6TO4";
            case NetInfo::TYPE_UNKNOWN:
                return "UNKNOWN";
            default:
                LBUNREACHABLE;
                return "ERROR";
            }
        }

        void setType( const std::string& strType )
        {
            if( strType == "ETHERNET" )
                type = NetInfo::TYPE_ETHERNET;
            else if( strType == "INFINIBAND" )
                type = NetInfo::TYPE_INFINIBAND;
            else if( strType == "LOOPBACK" )
                type = NetInfo::TYPE_LOOPBACK;
            else
                type = NetInfo::TYPE_UNKNOWN;
        }

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

        char dummy[24]; //!< Buffer for binary-compatible additions
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
