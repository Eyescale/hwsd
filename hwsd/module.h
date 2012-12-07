
/* Copyright (c) 2011, Stefan Eilemann <eile@eyescale.ch>
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

#ifndef HWSD_MODULE_H
#define HWSD_MODULE_H

#include <lunchbox/uuid.h>

#include <string>

namespace hwsd
{
namespace detail
{
    class Module;
}
    /** Base class for runtime-attached DSOs of a query implementation. */
    template< typename T >
    class Module
    {
    public:
        /** Register and construct a new module. @version 1.0 */
        Module();

        /** Destruct this module. @version 1.0 */
        virtual ~Module();

        /**
         * Announce the information about all found resources. The default
         * announcement goes to std::cout.
         *
         * @return true if announcement was successful
         * @version 1.0
         */
        virtual bool announce( const lunchbox::UUID& nodeID,
                               const std::string& session ) const;

        /** @return information about all found resources. @version 1.0 */
        virtual T discover() const = 0;

        static Module< T >* stack_;
        Module< T >* next_;
    };
}

#include "module.ipp" // template implementation

#endif // HWSD_MODULE_H
