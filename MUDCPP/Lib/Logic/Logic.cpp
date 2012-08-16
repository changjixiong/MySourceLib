// MUD Programming
// Ron Penton
// (C)2003
// SCChat.cpp - this is a simple chatter handler

#include "Logic.h"
#include "../UserDB/UserDB.h"
#include <exception>
#include <io.h>
#include <iostream>
#include <direct.h>

using namespace SocketLib;

// ------------------------------------------------------------------------
//  This handles incomming commands. Anything passed into this function
//  is assumed to be a complete command from a client.
// ------------------------------------------------------------------------
void Logic::Handle( string p_data )
{
    // get the users name
    string name = UserDatabase::find( m_connection )->name;

    // message is a command

    string command = BasicLib::ParseWord( p_data, 0 );
    string data = BasicLib::RemoveWord( p_data, 0 );

    if( command == "quit" )
    {
        CloseConnection( "has quit. Message: " + data +"\r\n");
		BasicLib::DeBugOut(UserDatabase::find( m_connection )->name + " quit");
        m_connection->Close();
    }
    // else it's just regular text.
    
	else
    {
		BasicLib::DeBugOut(string("send : ") + "unknow\\r\\n");
		m_connection->GetTranslator().SendString( *m_connection, "unknow\r\n" );		        
    }

}


// ------------------------------------------------------------------------
//  This notifies the handler that a connection is entering the state
// ------------------------------------------------------------------------
void Logic::Enter()
{
	//SendAll( UserDatabase::find( m_connection )->name + " entered\r\n" );	

	m_connection->GetTranslator().SendString( *m_connection, UserDatabase::find( m_connection )->name + " entered\r\n" );

}


// ------------------------------------------------------------------------
//  This notifies the handler that a connection is leaving the state.
// ------------------------------------------------------------------------
void Logic::Leave()
{
	UserDatabase::DeleteUser( m_connection );
}


// ------------------------------------------------------------------------
//  This notifies the handler that a connection has unexpectedly hung up.
// ------------------------------------------------------------------------
void Logic::Hungup()
{
    CloseConnection( "has hung up!" );
}

// ------------------------------------------------------------------------
//  This notifies the handler that a connection is being kicked due to 
//  flooding the server.
// ------------------------------------------------------------------------
void Logic::Flooded()
{
    CloseConnection( "has been kicked for flooding!" );
}

// ------------------------------------------------------------------------
//  Closes a connection, using a specified closing message
// ------------------------------------------------------------------------
void Logic::CloseConnection( const string& p_reason )
{
// 	SendAll( UserDatabase::find( m_connection )->name +
//         " " + p_reason );
}


// ------------------------------------------------------------------------
//  Sends a message to all connections
// ------------------------------------------------------------------------
void Logic::SendAll( const string& p_message )
{
    UserDatabase::iterator itr = UserDatabase::begin();

    // loop through every connection and send the message.
    while( itr != UserDatabase::end() )
    {
        itr->connection->GetTranslator().SendString( *itr->connection, p_message );
		++itr;
    }
}
