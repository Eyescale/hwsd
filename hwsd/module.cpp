
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
#include "gpuInfo.h"

namespace hwsd
{
namespace
{
static Module* stack_ = 0;
}

namespace detail
{
class Module
{
public:
    Module() : next_( 0 ) {}
    ~Module() {}

    hwsd::Module* next_;
};
}

Module::Module()
        : impl_( new detail::Module( ))
{
    if( !stack_ )
    {
        stack_ = this;
        return;
    }

    for( Module* module = stack_; module; module = module->impl_->next_ )
    {
        if( !module->impl_->next_ )
        {
            module->impl_->next_ = this;
            return;
        }
    }
}

Module::~Module()
{
    Module* previous = 0;
    for( Module* module = stack_; module; module = module->impl_->next_ )
    {
        if( module == this )
        {
            if( previous )
                previous->impl_->next_ = impl_->next_;
            else
                stack_ = impl_->next_;
            break;
        }
        previous = module;
    }
    delete impl_;
}

GPUInfos Module::discoverGPUs( FilterPtr filter )
{
    GPUInfos result;
    for( Module* module = stack_; module; module = module->impl_->next_ )
    {
        const GPUInfos infos = module->discoverGPUs_();
        for( GPUInfosCIter i = infos.begin(); i != infos.end(); ++i )
        {
            const GPUInfo& info = *i;
            if( !filter || (*filter)( result, info ))
                result.push_back( info );
        }
    }
    return result;
}

}
