
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

#ifndef HWSD_NODEINFO_H
#define HWSD_NODEINFO_H

#include <lunchbox/uuid.h>

#include <ostream>


namespace hwsd
{
    /** A structure containing node-specific information. */
    struct NodeInfo
    {
        /** Default constructor describing a local session. @version 1.0 */
        NodeInfo() : id(), session( "local" ) {}

        /** @return true if both informations are identical. @version 1.0 */
        bool operator == ( const NodeInfo& rhs ) const
            {
                return id == rhs.id && nodeName == rhs.nodeName &&
                       session == rhs.session;
            }

        /** @return true if both infos are not identical. @version 1.0 */
        bool operator != ( const NodeInfo& rhs ) const
            {
                return !(*this == rhs );
            }

        /** Random identifier if remote, UUID::ZERO if local. @version 1.0 */
        lunchbox::UUID id;

        /** System hostname if remote, empty for local. @version 1.0 */
        std::string nodeName;

        /** The session name: local, default or custom string. @version 1.0 */
        std::string session;

        char dummy[24]; //!< Buffer for binary-compatible additions
    };

    inline std::ostream& operator << ( std::ostream& os, const NodeInfo& info )
    {
        if( info.id != lunchbox::UUID::ZERO )
            os << "  NodeID    " << info.id << std::endl;
        if( !info.nodeName.empty( ))
            os << "  Nodename  " << info.nodeName << std::endl;
        if( !info.session.empty() && info.session != "local" )
            os << "  Session   " << info.session << std::endl;
        return os;
    }
}

#endif // HWSD_NODEINFO_H
