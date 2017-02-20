
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

#include "netInfo.h"

namespace hwsd
{
NetInfo::NetInfo()
    : type(TYPE_UNKNOWN)
    , linkspeed(defaultValue)
    , up(false)
{
}

bool NetInfo::operator==(const NetInfo& rhs) const
{
    return NodeInfo::operator==(rhs) && type == rhs.type && name == rhs.name &&
           hostname == rhs.hostname && hwAddress == rhs.hwAddress &&
           inetAddress == rhs.inetAddress && inet6Address == rhs.inet6Address &&
           linkspeed == rhs.linkspeed && up == rhs.up;
}

bool NetInfo::operator!=(const NetInfo& rhs) const
{
    return !(*this == rhs);
}

std::string NetInfo::getType() const
{
    switch (type)
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

void NetInfo::setType(const std::string& strType)
{
    if (strType == "ETHERNET")
        type = NetInfo::TYPE_ETHERNET;
    else if (strType == "INFINIBAND")
        type = NetInfo::TYPE_INFINIBAND;
    else if (strType == "LOOPBACK")
        type = NetInfo::TYPE_LOOPBACK;
    else
        type = NetInfo::TYPE_UNKNOWN;
}
}
