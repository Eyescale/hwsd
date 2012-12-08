
/*
  Copyright (c) 2011-2012, Stefan Eilemann <eile@eyescale.ch>

  This file is part of the HW-SD daemon.

  The HW-SD daemon is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the Free
  Software Foundation, either version 3 of the License, or (at your option) any
  later version.

  HW-SD is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with HW-SD. If not, see <http://www.gnu.org/licenses/>.
*/

#include <hwsd/hwsd.h>
#include <hwsd/gpuInfo.h>
#include <hwsd/netInfo.h>
#include <hwsd/version.h>

#ifdef HWSD_GPU_CGL
#  include <hwsd/gpu/cgl/module.h>
#endif
#ifdef HWSD_GPU_GLX
#  include <hwsd/gpu/glx/module.h>
#endif
#ifdef HWSD_GPU_WGL
#  include <hwsd/gpu/wgl/module.h>
#endif
#include <hwsd/gpu/dns_sd/module.h>

#include <hwsd/net/dns_sd/module.h>
#include <hwsd/net/sys/module.h>

#include <lunchbox/lunchbox.h>

#ifdef HWSD_USE_BOOST
#  include <boost/program_options/options_description.hpp>
#  include <boost/program_options/parsers.hpp>
#  include <boost/program_options/variables_map.hpp>
   namespace arg = boost::program_options;
#endif


int main( const int argc, char* argv[] )
{
    std::string session( "default" );

#ifdef HWSD_USE_BOOST
    const std::string applicationName = "Hardware service discovery daemon";
    arg::variables_map vm;
    arg::options_description desc( applicationName );
    desc.add_options()
        ( "help", "output this help message" )
        ( "version,v", "print version" )
        ( "session,s", arg::value< std::string >()->default_value( session ),
          "set session name" )
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
        std::cout << applicationName << " " << hwsd::Version::getString()
                  << "\n" << std::endl;
        return EXIT_SUCCESS;
    }

    if( vm.count( "session" ))
        session = vm["session"].as< std::string >();

    const bool daemon = vm.count( "daemon" ) > 0;
#else
    if( argc > 1 )
        std::cerr << "Ignoring command line options, compiled without "
                  << "boost::program_options support" << std::endl;
    const bool daemon = false;
#endif

#ifdef _WIN32
    const std::string executable = argv[0];
#else
    const std::string executable = basename( argv[0] );
#endif
    if( executable != "net_sd" )
    {
#ifdef HWSD_GPU_CGL
        hwsd::gpu::cgl::Module::use();
#endif
#ifdef HWSD_GPU_GLX
        hwsd::gpu::glx::Module::use();
#endif
#ifdef HWSD_GPU_WGL
        hwsd::gpu::wgl::Module::use();
#endif
        hwsd::gpu::dns_sd::Module::use();
        if( !hwsd::announceGPUs( session ))
        {
            std::cerr << "GPU announcement failed" << std::endl;
            return EXIT_FAILURE;
        }
    }

    if( executable != "gpu_sd" )
    {
        hwsd::net::dns_sd::Module::use();
        hwsd::net::sys::Module::use();
        if( !hwsd::announceNets( session ))
        {
            std::cerr << "Network announcement failed" << std::endl;
            return EXIT_FAILURE;
        }
    }

    if( daemon )
    {
#if LUNCHBOX_VERSION_GT(1, 5, 0)
        if( lunchbox::Log::setOutput( "hwsd.log" ))
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
