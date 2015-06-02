
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

#include <hwsd/nodeInfo.h>

#include <servus/servus.h>

#include <string>


namespace hwsd
{
namespace detail
{
namespace dns_sd
{

static std::string NODEID   = "NodeID";
static std::string NODENAME = "Hostname";
static std::string SESSION  = "Session";

class Module
{
public:
    Module( const std::string& serviceName, const std::string& resource )
        : service( serviceName )
        , _resource( resource )
    {}

    void announce( const NodeInfo& info )
    {
        setValue( NODEID, info.id, true );
        setValue( NODENAME, info.nodeName, true );
        setValue( SESSION, info.session, true );
    }

    void discover( const std::string& host, NodeInfo& info )
    {
        info.nodeName = host;
        if( !getValue( host, NODEID, info.id, true ))
            info.id = servus::make_uint128( info.nodeName.c_str( ));
        getValue( host, NODENAME, info.nodeName, true );
        getValue( host, SESSION, info.session, true );
    }

    template< class T >
    bool getValue( const std::string& host, const size_t index,
                   const std::string& key, T& value )
    {
        std::ostringstream out;
        out << _resource << index << " " << key;
        const std::string string = out.str();

        const std::string& data = service.get( host, string );
        if( data.empty( ))
            return false;

        std::istringstream in( data );
        in >> value;
        return true;
    }

    template< class T >
    bool getValue( const std::string& host, const std::string& key, T& value,
                   bool global = false )
    {
        std::ostringstream out;
        if( global )
            out << key;
        else
            out << _resource << " " << key;
        const std::string string = out.str();

        const std::string& data = service.get( host, string );
        if( data.empty( ))
            return false;

        std::istringstream in( data );
        in >> value;
        return true;
    }

    template< class T >
    void setValue( const size_t index, const std::string& key, const T& value )
    {
        std::ostringstream out;
        out << _resource << index << " " << key;
        const std::string string = out.str();

        out.str("");
        out << value;
        service.set( string, out.str( ));
    }

    template< class T >
    void setValue( const std::string& key, const T& value, bool global = false )
    {
        std::ostringstream out;
        if( global )
            out << key;
        else
            out << _resource << " " << key;
        const std::string string = out.str();

        out.str("");
        out << value;
        service.set( string, out.str( ));
    }

    servus::Servus service;

private:
    std::string _resource;
};
}


}
}
