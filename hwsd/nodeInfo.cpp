
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

#include "nodeInfo.h"
#include "hwsd.h"

namespace hwsd
{
NodeInfo::NodeInfo()
    : id( getLocalNodeID( )), session( "local" )
{}

bool NodeInfo::operator == ( const NodeInfo& rhs ) const
{
    return nodeName == rhs.nodeName && session == rhs.session;
}

bool NodeInfo::operator != ( const NodeInfo& rhs ) const
{
    return !(*this == rhs );
}

std::ostream& operator << ( std::ostream& os, const NodeInfo& info )
{
        os << "  NodeID    " << info.id << std::endl;
    if( !info.nodeName.empty( ))
        os << "  Nodename  " << info.nodeName << std::endl;
    if( !info.session.empty() && info.session != "local" )
        os << "  Session   " << info.session << std::endl;
    return os;
}

}
