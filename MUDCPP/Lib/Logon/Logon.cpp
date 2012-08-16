// MUD Programming
// Ron Penton
// (C)2003
// SCLogon.cpp - A simple chat server's logon handler.
// 

#include "Logon.h"
#include "../Logic/Logic.h"
#include "../UserDB/UserDB.h"

using namespace SocketLib;

// ------------------------------------------------------------------------
//  This handles incomming commands. Anything passed into this function
//  is assumed to be a complete command from a client.
// ------------------------------------------------------------------------
void Logon::Handle( string p_data )
{
    // record the connection pointer for future reference.
    Connection* conn = m_connection;

	// make sure name is valid
    if( !UserDatabase::IsValidName( p_data ) )
    {
		BasicLib::DeBugOut(string("send : ") + "invalid username\\r\\n");
        conn->GetTranslator().SendString( *conn, "invalid username\r\n");				
        return;
    }

    // make sure name doesn't exist already
    if( UserDatabase::HasUser( p_data ) )
    {
		BasicLib::DeBugOut(string("send : ") + "user is existence\\r\\n");
        conn->GetTranslator().SendString( *conn, "user is existence\r\n");		
        return;
    }

    // add the user
    UserDatabase::AddUser( conn, p_data );
	//BasicLib::DeBugOut(string("send : ") + p_data + " is logining");
    conn->GetTranslator().SendString( *conn, "" +p_data+" is logining\r\n");
	
    // set the new state machine
    conn->RemoveHandler();
    conn->AddHandler( new Logic( *conn ) );
}


// ------------------------------------------------------------------------
//  This notifies the handler that a connection is entering the state
// ------------------------------------------------------------------------
void Logon::Enter()
{
    // welcome the new user	
	BasicLib::DeBugOut(string("send : ") + "ssname:\\r\\n");
    m_connection->GetTranslator().SendString( *m_connection, "ssname:\r\n");	
}



