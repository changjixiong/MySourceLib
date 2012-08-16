// MUD Programming
// Ron Penton
// (C)2003
// SCChat.h - this is a simple chatter handler


#ifndef SCCHAT_H
#define SCCHAT_H

#include "../SocketLib/Connection.h"
#include "../SocketLib/ConnectionHandler.h"
#include <string>

using SocketLib::Connection;
using SocketLib::ConnectionHandler;

using std::string;

//const int FileBlockLen = 512;

class Logic : public ConnectionHandler
{
    //typedef Telnet::handler thandler;
public:

    Logic( Connection& p_conn )
        : ConnectionHandler( p_conn ) {};

	~Logic()
	{
		if (filein.is_open())
		{
			filein.clear();
			filein.close();
		}

	}

    // ------------------------------------------------------------------------
    //  This handles incomming commands. Anything passed into this function
    //  is assumed to be a complete command from a client.
    // ------------------------------------------------------------------------
    void Handle( string p_data );

    // ------------------------------------------------------------------------
    //  This notifies the handler that a connection is entering the state
    // ------------------------------------------------------------------------
    void Enter();

    // ------------------------------------------------------------------------
    //  This notifies the handler that a connection is leaving the state.
    // ------------------------------------------------------------------------
    void Leave();

    // ------------------------------------------------------------------------
    //  This notifies the handler that a connection has unexpectedly hung up.
    // ------------------------------------------------------------------------
    void Hungup();

    // ------------------------------------------------------------------------
    //  This notifies the handler that a connection is being kicked due to 
    //  flooding the server.
    // ------------------------------------------------------------------------
    void Flooded();

    // ------------------------------------------------------------------------
    //  Closes a connection, using a specified closing message
    // ------------------------------------------------------------------------
    void CloseConnection( const string& p_reason );

    // ------------------------------------------------------------------------
    //  Sends a message to all connections
    // ------------------------------------------------------------------------
    void SendAll( const string& p_message );

	int GetFileList(string & strFilelist, const string & strPath);
	int GetFileFromClient(const string &strFile);
	int SendFileToClient(const string &strFile);

	ifstream filein;
	ofstream fileout;

	string	strSendFileName;
	string	strGetFileName;
	long	nFileInLen;
	long	nFileOutLen;

};

#endif
