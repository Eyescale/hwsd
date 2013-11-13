
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

#ifndef HWSD_NODEINFO_H
#define HWSD_NODEINFO_H

#include <hwsd/api.h>
#include <lunchbox/uint128_t.h>

#include <ostream>


namespace hwsd
{
/** A structure containing node-specific information. */
struct NodeInfo
{
    /** Default constructor describing a local session. @version 1.0 */
    HWSD_API NodeInfo();

    /** @return true if both informations are identical. @version 1.0 */
    HWSD_API bool operator == ( const NodeInfo& rhs ) const;

    /** @return true if both infos are not identical. @version 1.0 */
    HWSD_API bool operator != ( const NodeInfo& rhs ) const;

    /** @return true if this resource was discovered locally. @version 1.1.1 */
    HWSD_API bool isLocal() const;

    /** @return true if this resource was discovered locally. @version 1.1.1 */
    HWSD_API static bool isLocal( const std::string& name );

    /** @return the session name for local resources. @version 1.1.1 */
    static std::string getLocalSession() { return "local"; }

    /** Random unique ID identifing the origin node. @version 1.0 */
    lunchbox::uint128_t id;

    /** System hostname if remote, empty for local. @version 1.0 */
    std::string nodeName;

    /** The session name: local, default or custom string. @version 1.0 */
    std::string session;

    char dummy[32]; //!< Buffer for binary-compatible additions
};

HWSD_API std::ostream& operator << ( std::ostream& os,
                                     const NodeInfo& info );
}

#endif // HWSD_NODEINFO_H
