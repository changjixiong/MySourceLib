// MUD Programming
// Ron Penton
// (C)2003
// ConnectionHandler.h - This is a generic interface class designed to handle
//                       data from a connection policy class.


#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <string>

#include "SocketLibTypes.h"
#include "Connection.h"

using namespace  std;

namespace SocketLib
{

class Connection;

class ConnectionHandler
{

public:

    // ------------------------------------------------------------------------
    //  Constructor, records pointer to the connection.
    // ------------------------------------------------------------------------
    ConnectionHandler( Connection& p_conn ) : m_connection( &p_conn ) {}
    virtual ~ConnectionHandler() {};

    // ------------------------------------------------------------------------
    //  This handles incomming commands. Anything passed into this function
    //  is assumed to be a complete command from a client.
    // ------------------------------------------------------------------------
    virtual void Handle( string p_data ) {}

    // ------------------------------------------------------------------------
    //  This notifies the handler that a connection is entering the state
    // ------------------------------------------------------------------------
    virtual void Enter() {}

    // ------------------------------------------------------------------------
    //  This notifies the handler that a connection is leaving the state.
    // ------------------------------------------------------------------------
    virtual void Leave() {}

    // ------------------------------------------------------------------------
    //  This notifies the handler that a connection has unexpectedly hung up.
    // ------------------------------------------------------------------------
    virtual void Hungup() {}

    // ------------------------------------------------------------------------
    //  This notifies the handler that a connection is being kicked due to 
    //  flooding the server.
    // ------------------------------------------------------------------------
    virtual void Flooded() {}

protected:
    Connection* m_connection;

};


}   // end namespace SocketLib
#endif
