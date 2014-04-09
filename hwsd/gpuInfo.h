
/* Copyright (c) 2011-2013, Stefan Eilemann <eile@eyescale.ch>
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

#ifndef HWSD_GPUINFO_H
#define HWSD_GPUINFO_H

#include <hwsd/nodeInfo.h>  // base class

#include <climits>
#include <cstring>
#include <iostream>
#include <string>


namespace hwsd
{
/** A structure containing GPU-specific information. */
struct GPUInfo : public NodeInfo
{
    /** A non-enumerated port or device. @version 1.0 */
    static const unsigned defaultValue = UINT_MAX;

    /** Process runs under VirtualGL. @version 1.1.2 */
    static const unsigned FLAG_VIRTUALGL = 0x1;

    /** VirtualGL redirect GPU. @version 1.1.2 */
    static const unsigned FLAG_VIRTUALGL_DISPLAY = 0x2;

    /** Default constructor pointing to the default display. @version 1.0 */
    GPUInfo();

    /**
     * Constructor pointing to default display of a specific GPU type.
     *
     * The information name is a type code of four characters. The passed string
     * is formatted accordingly.
     *
     * @param name the type of the GPU.
     * @version 1.0
     */
    GPUInfo( const std::string& name );

    /** Invalidate the pixel viewport. @version 1.0 */
    void invalidatePVP();

    /** @return true if both informations are identical. @version 1.0 */
    bool operator == ( const GPUInfo& rhs ) const;

    /** @return true if both infos are not identical. @version 1.0 */
    bool operator != ( const GPUInfo& rhs ) const;

    /** @return the type name string of this information. @version 1.0 */
    std::string getName() const;

    /** Four-character code of the GPU type. @version 1.0 */
    unsigned type;

    /** The display (GLX) or ignored (WGL, AGL). @version 1.0 */
    unsigned port;

    /**
     * The screen (GLX, WGL), GPU (WGLn, WGLa) or virtual screen (AGL).
     * @version 1.0
     */
    unsigned device;

    /** The size and location of the GPU (x,y,w,h). @version 1.0 */
    int pvp[4];

    unsigned flags; //!< bitmask of additional GPU capabilities
    unsigned unused; //!< @internal
};

inline std::ostream& operator << ( std::ostream& os, const GPUInfo& info )
{
    os << "GPUInfo\n" << static_cast< const NodeInfo& >( info );
    if( !info.getName().empty( ))
        os << "  Type      " << info.getName() << std::endl;
    if( info.port != GPUInfo::defaultValue )
        os << "  Port      " << info.port << std::endl;
    if( info.device != GPUInfo::defaultValue )
        os << "  Device    " << info.device << std::endl;
    if( info.pvp[2] >0 && info.pvp[3] > 0 )
        os << "  Viewport  [" << info.pvp[0] << ' ' << info.pvp[1] << ' '
           << info.pvp[2] << ' ' << info.pvp[3] << ']' << std::endl;
    if( info.flags != 0 )
        os << "  Flags     "
           << (info.flags&GPUInfo::FLAG_VIRTUALGL ? "VirtualGL" : "")
           << (info.flags&GPUInfo::FLAG_VIRTUALGL_DISPLAY ? "Display" : "")
           << std::endl;
    return os;
}
}

#endif // HWSD_GPUINFO_H
