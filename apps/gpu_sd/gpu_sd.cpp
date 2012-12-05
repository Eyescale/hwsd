
/*
  Copyright (c) 2011-2012, Stefan Eilemann <eile@eyescale.ch>

  This file is part of the GPU-SD daemon.

  The GPU-SD daemon is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the Free
  Software Foundation, either version 3 of the License, or (at your option) any
  later version.

  GPU-SD is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with GPU-SD. If not, see <http://www.gnu.org/licenses/>.
*/

#include <gpusd/gpuInfo.h>
#include <gpusd/module.h>
#include <gpusd/version.h>
#ifdef GPUSD_CGL
#  include <gpusd/cgl/module.h>
#endif
#ifdef GPUSD_GLX
#  include <gpusd/glx/module.h>
#endif
#ifdef GPUSD_WGL
#  include <gpusd/wgl/module.h>
#endif

#include <lunchbox/lunchbox.h>

#include <cstdio>
#include <cstdlib>
#include <sstream>

#ifdef GPUSD_USE_BOOST
#  include <boost/program_options/options_description.hpp>
#  include <boost/program_options/parsers.hpp>
#  include <boost/program_options/variables_map.hpp>
   namespace arg = boost::program_options;
#endif

using gpusd::GPUInfo;
using gpusd::GPUInfos;

static void setKey( lunchbox::Servus& service, const size_t gpuIndex,
                    const std::string& name, const unsigned value )
{
    std::ostringstream out;
    out << "GPU" << gpuIndex << " " << name;
    const std::string string = out.str();

    out.str("");
    out << value;
    service.set( string, out.str( ));
}

static void setKeys( lunchbox::Servus& service, const GPUInfos& gpus,
                     const std::string& session, const std::string& hostname )
{
    service.set( "Session", session );
    service.set( "Hostname", hostname );

    std::ostringstream out;
    out << gpus.size();
    service.set( "GPU Count", out.str( ));

    for( gpusd::GPUInfosCIter i = gpus.begin(); i != gpus.end(); ++i )
    {
        const GPUInfo& info = *i;
        const size_t index = i - gpus.begin();

        // GPU<integer> Type=GLX | WGL | WGLn | CGL
        out.str("");
        out << "GPU" << index << " Type";
        service.set( out.str(), info.getName( ));

        if( info.port != GPUInfo::defaultValue )
            // GPU<integer> Port=<integer> // X11 display number, 0 otherwise
            setKey( service, index, "Port", info.port );

        if( info.device != GPUInfo::defaultValue )
            // GPU<integer> Device=<integer> // X11 display number, 0 otherwise
            setKey( service, index, "Device", info.device );

        if( info.pvp[2] > 0 && info.pvp[3] > 0 )
        {
            setKey( service, index, "X", info.pvp[0] );
            setKey( service, index, "Y", info.pvp[1] );
            setKey( service, index, "Width", info.pvp[2] );
            setKey( service, index, "Height", info.pvp[3] );
        }

        if( info.flags != 0 )
            setKey( service, index, "Flags", info.flags );
    }
}

int main( const int argc, const char* argv[] )
{
    std::string session( "default" );
    std::string hostname;
    unsigned short port = 4242;

#ifdef GPUSD_USE_BOOST
    const std::string applicationName = "GPU service discovery daemon";
    arg::variables_map vm;
    arg::options_description desc( applicationName );
    desc.add_options()
        ( "help", "output this help message" )
        ( "version,v", "print version" )
        ( "session,s", arg::value< std::string >()->default_value( session ),
          "set session name" )
        ( "hostname,h", arg::value< std::string >(), "set hostname" )
        ( "port,p", arg::value< unsigned short >()->default_value( port ),
          "set listening port" )
        ( "daemon,d", "run as daemon" );

    try
    {
        arg::store( arg::parse_command_line( argc, argv, desc ), vm );
        arg::notify( vm );
    }
    catch( ... )
    {
        std::cout << desc << std::endl;
        return EXIT_FAILURE;
    }
    if( vm.count( "help" ))
    {
        std::cout << desc << std::endl;
        return EXIT_SUCCESS;
    }
    if( vm.count( "version" ))
    {
        std::cout << applicationName << " " << gpusd::Version::getString()
                  << "\n" << std::endl;
        return EXIT_SUCCESS;
    }

    if( vm.count( "session" ))
        session = vm["session"].as< std::string >();
    if( vm.count( "hostname" ))
        hostname = vm["hostname"].as< std::string >();
    if( vm.count( "port" ))
        port = vm["port"].as< unsigned short >();

    const bool daemon = vm.count( "daemon" ) > 0;
#else
    if( argc > 1 )
        std::cerr << "Ignoring command line options, compiled without "
                  << "boost::program_options support" << std::endl;
    const bool daemon = false;
#endif

#ifdef GPUSD_CGL
    gpusd::cgl::Module::use();
#endif
#ifdef GPUSD_GLX
    gpusd::glx::Module::use();
#endif
#ifdef GPUSD_WGL
    gpusd::wgl::Module::use();
#endif

    const GPUInfos& gpus = gpusd::Module::discoverGPUs();
    if( gpus.empty( ))
    {
        std::cerr << "No GPUs found, quitting" << std::endl;
        return EXIT_FAILURE;
    }

    lunchbox::Servus service( "_gpu-sd._tcp" );
    setKeys( service, gpus, session, hostname );
    if( !service.announce( port, "" ))
    {
        std::cerr << "Service announcement failed" << std::endl;
        return EXIT_FAILURE;
    }

    if( daemon )
    {
#if LUNCHBOX_VERSION_GT(1, 5, 0)
        if( lunchbox::Log::setOutput( "gpusd.log" ))
            lunchbox::daemonize();
#else
        LBWARN << "Ignoring daemon request, need Lunchbox >= 1.5.1, got "
               << lunchbox::Version::getString() << std::endl;
#endif
        for( ;; )
            lunchbox::sleep( 0xFFFFFFFFu );
    }

    std::cout << "Press <Enter> to quit" << std::endl;
    getchar();
    return EXIT_SUCCESS;
}
