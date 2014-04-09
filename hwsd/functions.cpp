
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

#include "functions.h"
#include "gpuInfo.h"
#include "netInfo.h"
#include "module.h"

namespace hwsd
{

template<> GPUModule* GPUModule::stack_ = 0;
template<> NetModule* NetModule::stack_ = 0;

lunchbox::UUID nodeID_ = lunchbox::make_UUID();


bool announceGPUInfos( const std::string& session )
{
    for( GPUModule* module = GPUModule::stack_; module; module = module->next_ )
    {
        if( !module->announce( session ))
            return false;
    }
    return true;
}

GPUInfos discoverGPUInfos( FilterPtr filter )
{
    GPUInfos result;
    for( GPUModule* module = GPUModule::stack_; module; module = module->next_ )
    {
        const GPUInfos& infos = module->discover();
        for( GPUInfosCIter i = infos.begin(); i != infos.end(); ++i )
        {
            const GPUInfo& info = *i;
            if( !filter || (*filter)( result, info ))
                result.push_back( info );
        }
    }
    return result;
}

bool announceNetInfos( const std::string& session )
{
    for( NetModule* module = NetModule::stack_; module; module = module->next_ )
    {
        if( !module->announce( session ))
            return false;
    }
    return true;
}

NetInfos discoverNetInfos( FilterPtr filter )
{
    NetInfos result;
    for( NetModule* module = NetModule::stack_; module; module = module->next_ )
    {
        const NetInfos& infos = module->discover();
        for( NetInfosCIter i = infos.begin(); i != infos.end(); ++i )
        {
            const NetInfo& info = *i;
            if( !filter || (*filter)( result, info ))
                result.push_back( info );
        }
    }
    return result;
}

const lunchbox::UUID& getLocalNodeID()
{
    return nodeID_;
}

}
