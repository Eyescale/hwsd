
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

#include "gpuInfo.h"

namespace hwsd
{
    GPUInfo::GPUInfo()
        : type( 0 )
        , port( 0 )
        , device( 0 )
        , flags( 0 )
        , unused( 0 )
    {
        invalidatePVP();
    }

    GPUInfo::GPUInfo( const std::string& name )
        : type( 0 )
        , port( 0 )
        , device( 0 )
        , flags( 0 )
        , unused( 0 )
    {
        invalidatePVP();
        strncpy( reinterpret_cast< char* >( &type ), name.c_str(), 4 );
    }

    void GPUInfo::invalidatePVP()
    {
        pvp[0] = 0;
        pvp[1] = 0;
        pvp[2] = -1;
        pvp[3] = -1;
    }

    bool GPUInfo::operator == ( const GPUInfo& rhs ) const
    {
        return ( NodeInfo::operator==( rhs ) && type == rhs.type &&
                 port == rhs.port && device == rhs.device &&
                 pvp[0] == rhs.pvp[0] && pvp[1] == rhs.pvp[1] &&
                 pvp[2] == rhs.pvp[2] && pvp[3] == rhs.pvp[3] );
    }

    bool GPUInfo::operator != ( const GPUInfo& rhs ) const
    {
        return !(*this == rhs );
    }

    std::string GPUInfo::getName() const
    {
        return std::string( reinterpret_cast<const char*>( &type ), 4 );
    }
}
