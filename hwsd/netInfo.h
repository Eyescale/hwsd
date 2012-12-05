
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

#ifndef HWSD_NETINFO_H
#define HWSD_NETINFO_H

#include <ostream>


namespace hwsd
{
    /** A structure containing network-specific information. */
    struct NetInfo
    {
        /** Default constructor pointing to the default display. @version 1.0 */
        NetInfo() {}

        /** @return true if both informations are identical. @version 1.0 */
        bool operator == ( const NetInfo& rhs ) const
            {
                return true;
            }

        /** @return true if both infos are not identical. @version 1.0 */
        bool operator != ( const NetInfo& rhs ) const
            {
                return !(*this == rhs );
            }

        char dummy[24]; //!< Buffer for binary-compatible additions
    };

    inline std::ostream& operator << ( std::ostream& os, const NetInfo& info )
    {
        return os;
    }
}

#endif // HWSD_NETINFO_H
