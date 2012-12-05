
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

#ifndef HWSD_GPU_DNS_SD_MODULE_H
#define HWSD_GPU_DNS_SD_MODULE_H

#include <hwsd/module.h> // base class

namespace hwsd
{
namespace gpu
{
namespace dns_sd
{
    /** The DNS_SD implementation for remote GPU discovery. */
    class Module : public hwsd::Module
    {
    public:
        /** Instantiate an AGL discovery module for the process. */
        static HWSD_API void use();

        /** Unload the AGL discovery module for the process. */
        static HWSD_API void dispose();

    protected:
        virtual GPUInfos discoverGPUs_() const;

    private:
        Module() : hwsd::Module() {}
        virtual ~Module() {}
    };
}
}
}
#endif // HWSD_GPU_DNS_SD_MODULE_H

