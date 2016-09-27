
/*
 * Copyright (c) 2011-2013, Stefan Eilemann <eile@eyescale.ch>
 *
 * This file is part of the HW-SD discovery tool.
 *
 * The HW-SD discovery tool is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * HW-SD is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HW-SD. If not, see <http://www.gnu.org/licenses/>.
 */


#include <hwsd/functions.h>
#include <hwsd/gpuInfo.h>
#include <hwsd/gpu/dns_sd/module.h>
#include <hwsd/netInfo.h>
#include <hwsd/version.h>
#include <hwsd/net/dns_sd/module.h>
#include <hwsd/net/sys/module.h>
#ifdef HWSD_GPU_CGL
#  include <hwsd/gpu/cgl/module.h>
#endif
#ifdef HWSD_GPU_GLX
#  include <hwsd/gpu/glx/module.h>
#endif
#ifdef HWSD_GPU_WGL
#  include <hwsd/gpu/wgl/module.h>
#endif

#include <lunchbox/file.h>

#ifdef HWSD_USE_BOOST
#  pragma warning( disable: 4275 )
#  include <boost/program_options/options_description.hpp>
#  include <boost/program_options/parsers.hpp>
#  include <boost/program_options/variables_map.hpp>
#  pragma warning( default: 4275 )
   namespace arg = boost::program_options;
#endif

int main( int argc, const char * argv[] )
{
    std::string session;

#ifdef HWSD_USE_BOOST
    const std::string applicationName = "Hardware service discovery browser";
    arg::variables_map vm;

    arg::options_description desc( applicationName );
    desc.add_options()
        ( "help", "output this help message" )
        ( "version,v", "print version" )
        ( "session,s", arg::value< std::string >()->default_value( session ),
          "filter results by session name" );
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

#else
    if( argc > 1 )
        std::cerr << "Ignoring command line options, compiled without "
                  << "boost::program_options support" << std::endl;
#endif

    const std::string& executable = lunchbox::getFilename( argv[0] );

    if( executable != "net_sd_list" && executable != "gpu_sd_list" )
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
        hwsd::net::sys::Module::use();
    }

    hwsd::FilterPtr filter( new hwsd::DuplicateFilter );
    if( !session.empty( ))
        filter = filter | new hwsd::SessionFilter( session );

    if( executable != "net_sd_list" )
    {
        hwsd::gpu::dns_sd::Module::use();
        const hwsd::GPUInfos& gpus = hwsd::discoverGPUInfos( filter );
        for( hwsd::GPUInfosCIter i = gpus.begin(); i != gpus.end(); ++i )
            std::cout << *i << std::endl;
    }

    if( executable != "gpu_sd_list" )
    {
        hwsd::net::dns_sd::Module::use();
        const hwsd::NetInfos& nets = hwsd::discoverNetInfos( filter );
        for( hwsd::NetInfosCIter i = nets.begin(); i != nets.end(); ++i )
            std::cout << *i << std::endl;
    }

    return 0;
}
