
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

//#include <AGL/agl.h>
#include <Carbon/Carbon.h>

#include <deque>

#define MAX_GPUS 32

namespace hwsd
{
namespace gpu
{
namespace cgl
{
namespace
{
static Module* instance = 0;
}

void Module::use()
{
    if (!instance)
        instance = new Module;
}

void Module::dispose()
{
    delete instance;
    instance = 0;
}

GPUInfos Module::discover() const
{
    CGDirectDisplayID displayIDs[MAX_GPUS];
    CGDisplayCount nDisplays = 0;

    if (CGGetOnlineDisplayList(MAX_GPUS, displayIDs, &nDisplays) !=
        kCGErrorSuccess)
    {
        return GPUInfos();
    }

    const CGDirectDisplayID mainDisplayID = CGMainDisplayID();
    for (CGDisplayCount i = 0; i < nDisplays; ++i)
    {
        if (displayIDs[i] != mainDisplayID)
            continue;

        GPUInfo info("CGL");
        const CGRect displayRect = CGDisplayBounds(displayIDs[i]);

        info.device = i;
        info.pvp[0] = int32_t(displayRect.origin.x);
        info.pvp[1] = int32_t(displayRect.origin.y);
        info.pvp[2] = int32_t(displayRect.size.width);
        info.pvp[3] = int32_t(displayRect.size.height);

        return GPUInfos(1, info);
    }

    return GPUInfos();
}
}
}
}
