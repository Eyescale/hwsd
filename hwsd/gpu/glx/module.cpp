
/* Copyright (c) 2011-2012, Stefan Eilemann <eile@eyescale.ch>
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

#include <hwsd/gpuInfo.h>

#include <X11/Xlib.h>
#include <GL/glx.h>
#include <cstdlib>
#include <limits>
#include <sstream>

#define TRY_PORTS 10

namespace hwsd
{
namespace gpu
{
namespace glx
{
namespace
{
static Module* instance = 0;

static bool getGPUInfo_( Display* display, GPUInfo& info )
{
    int major, event, error;
    if( !display || !XQueryExtension( display, "GLX", &major, &event, &error ))
        return false;

    std::string displayString = DisplayString( display );
    const size_t colonPos = displayString.find( ':' );
    if( colonPos != std::string::npos )
    {
        const std::string displayNumber = displayString.substr( colonPos+1 );
        info.port = atoi( displayNumber.c_str( ));
        info.device = DefaultScreen( display );
    }

    info.pvp[0] = 0;
    info.pvp[1] = 0;
    info.pvp[2] = DisplayWidth(  display, DefaultScreen( display ));
    info.pvp[3] = DisplayHeight( display, DefaultScreen( display ));

    const char* vendor = glXGetClientString( display, GLX_VENDOR );
    if( vendor && std::string( vendor ) == "VirtualGL" )
    {
        info.flags |= GPUInfo::FLAG_VIRTUALGL;
        const char* vglDisplayEnv = getenv( "VGL_DISPLAY" );
        std::string vglDisplay( vglDisplayEnv ? vglDisplayEnv : ":0" );
        const bool vglHasScreen = vglDisplay[ vglDisplay.size() - 2 ] == '.';

        std::stringstream xDisplay;
        xDisplay << DisplayString( display );
        if( xDisplay.str() == ":0" && vglHasScreen )
            xDisplay << "." << DefaultScreen( display );

        const std::string& xDisplayStr = xDisplay.str();
        const bool xHasScreen = xDisplayStr[ xDisplayStr.size() - 2 ] == '.';
        if( xHasScreen && !vglHasScreen )
            vglDisplay += ".0";

        if( vglDisplay == xDisplayStr )
            info.flags |= GPUInfo::FLAG_VIRTUALGL_DISPLAY;
    }

    if ( XQueryExtension( display, "VNC-EXTENSION", &major, &event, &error ))
        info.flags |= GPUInfo::FLAG_VNC;

    return true;
}

static bool queryDisplay_( const std::string& display, GPUInfo& info )
{
    ::Display* xDisplay = XOpenDisplay( display.c_str( ));
    if( !xDisplay )
        return false;

    int major, event, error;
    if( !XQueryExtension( xDisplay, "GLX", &major, &event, &error ))
    {
        XCloseDisplay( xDisplay );
        return false;
    }

    getGPUInfo_( xDisplay, info );
    XCloseDisplay( xDisplay );
    return true;
}

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

GPUInfos Module::discover() const
{
    GPUInfos result;
    GPUInfo defaultInfo( "GLX" );

    const char* displayEnv = getenv( "DISPLAY" );
    if( displayEnv && displayEnv[0] != '\0' )
    {
        const std::string display( displayEnv );
        if( queryDisplay_( display, defaultInfo ))
        {
            defaultInfo.flags = GPUInfo::FLAG_DEFAULT;
            result.push_back( defaultInfo );
#ifdef __APPLE__
            // OS X only has one X server, but launchd magic makes duplicate
            // detection hard
            return result;
#endif
        }
    }

    // try x servers :0 - :n
    for( unsigned i = 0; i < std::numeric_limits< unsigned >::max() ; ++i )
        // x screens :n.0 - :n.m
        for( unsigned j = 0; j < std::numeric_limits< unsigned >::max(); ++j )
        {
            std::stringstream stream;
            stream <<  ':' << i << '.' << j;

            GPUInfo info( "GLX" );
            if( queryDisplay_( stream.str(), info ))
            {
                if( info != defaultInfo )
                    result.push_back( info );
            }
            else if( j == 0 && i > TRY_PORTS )
                // X Server does not exist, stop query
                return result;
            else // X Screen does not exist, try next server
                break;
        }
    return result;
}

}
}
}
