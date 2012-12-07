
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

#include <iostream>

namespace hwsd
{

template< typename T >
Module< T >::Module()
    : next_( 0 )
{
    if( !stack_ )
    {
        stack_ = this;
        return;
    }

    for( Module* module = stack_; module; module = module->next_ )
    {
        if( !module->next_ )
        {
            module->next_ = this;
            return;
        }
    }
}

template< typename T >
Module< T >::~Module()
{
    Module* previous = 0;
    for( Module* module = stack_; module; module = module->next_ )
    {
        if( module == this )
        {
            if( previous )
                previous->next_ = next_;
            else
                stack_ = next_;
            break;
        }
        previous = module;
    }
}

template< typename T >
bool Module< T >::announce( const lunchbox::UUID& nodeID,
                            const std::string& session ) const
{
    const T& resources = discover();
    for( typename T::const_iterator i = resources.begin();
         i != resources.end(); ++i )
    {
        typename T::value_type info = *i;
        info.id = nodeID;
        info.session = session;
        std::cout << info << std::endl;
    }
    return true;
}

}
