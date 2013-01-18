
/* Copyright (c) 2012, Daniel Nachbaur <danielnachbaur@gmail.com>
 *               2013, Stefan.Eilemann@epfl.ch
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

#ifndef HWSD_NET_SYS_MODULE_H
#define HWSD_NET_SYS_MODULE_H

#include <hwsd/api.h>
#include <hwsd/module.h> // base class
#include <hwsd/types.h>

namespace hwsd
{
namespace net
{
namespace sys
{
    /**
     * The low-level implementation for local network interface discovery.
     */
    class Module : public NetModule
    {
    public:
        /** Instantiate the sys network discovery module for the process. */
        static HWSD_API void use();

        /** Unload the sys network discovery module for the process. */
        static HWSD_API void dispose();

    protected:
        virtual NetInfos discover() const;

    private:
        Module() : NetModule() {}
        virtual ~Module() {}

        NetInfos _discoverWin32() const;
        NetInfos _discoverPosix() const;
    };
}
}
}

#endif // HWSD_NET_SYS_MODULE_H
