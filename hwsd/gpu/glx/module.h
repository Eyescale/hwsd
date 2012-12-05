
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

#ifndef HWSD_GPU_GLX_MODULE_H
#define HWSD_GPU_GLX_MODULE_H

#include <hwsd/api.h>
#include <hwsd/module.h> // base class
#include <hwsd/types.h>

namespace hwsd
{
namespace gpu
{
namespace glx
{
    /**
     * The GLX implementation for local GPU discovery.
     *
     * The GLX module tries to open X server :0 to :9. For each server which can
     * be opened, all available screens on the server are added as a separate
     * GPUInfo.
     */
    class Module : public hwsd::Module< GPUInfos >
    {
    public:
        /** Instantiate an GLX discovery module for the process. */
        static HWSD_API void use();

        /** Unload the GLX discovery module for the process. */
        static HWSD_API void dispose();

    protected:
        virtual GPUInfos discover() const;

    private:
        Module() : hwsd::Module< GPUInfos >() {}
        virtual ~Module() {}
    };
}
}
}
#endif // HWSD_GPU_GLX_MODULE_H
