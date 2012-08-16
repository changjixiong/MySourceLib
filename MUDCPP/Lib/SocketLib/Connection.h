// MUD Programming
// Ron Penton
// (C)2003
// Connection.h - This file contains the header for the Connection class,
//                which is an advanced version of a data socket.
//                Connections have functions for data flow management and
//                buffering. To properly use the buffering features,
//                the socket must be set to non-blocking mode.


#ifndef CONNECTION_H
#define CONNECTION_H

#include <stack>
#include "../BasicLib/BasicLib.h"
#include "SocketLibTypes.h"
#include "SocketLibSocket.h"
#include "ConnectionHandler.h"
#include "Translator.h"

namespace SocketLib
{

// This is the default buffer size of the connection class
const static int BUFFERSIZE = 1024;
const static int TIMECHUNK = 16;

class ConnectionHandler;
class Translator;

class Connection : public DataSocket
{

public:
    // ------------------------------------------------------------------------
    // Constructors. One constructs a normal connection, and the other accepts
    // a DataSocket to use as a "template" for the connection
    // ------------------------------------------------------------------------
    Connection();
    Connection( DataSocket& p_socket );

protected:
    void Initialize();


public:

    // ------------------------------------------------------------------------
    // This gets the amount of time that has passed, in seconds, since the
    // last time data was successfully sent, if there is data still queued,
    // or 0 if there is no data queued. This allows the server to check for
    // deadlocked clients (ie: a client crashed and is not receiving data).
    // ------------------------------------------------------------------------
    BasicLib::sint64 GetLastSendTime() const;
    
    // ------------------------------------------------------------------------
    // Gets the amount of time in seconds since data was last received.
    // ------------------------------------------------------------------------
    inline BasicLib::sint64 GetLastReceiveTime() const
    {
        return m_lastReceiveTime;
    }


    // ------------------------------------------------------------------------
    // This "closes" the connection. This just sets a boolean telling the
    // connection that it should be closed when the connection manager
    // gets to it.
    // ------------------------------------------------------------------------
    inline void Close()     { m_closed = true; }

    // ------------------------------------------------------------------------
    // This physically closes the underlying socket.
    // ------------------------------------------------------------------------
    inline void CloseSocket() 
    { 
        DataSocket::Close(); 

        // tell the top handler (if it exists) that the connection has left
        // that state
        ClearHandlers();

		if (p_translator)
		{
			delete p_translator;
			p_translator = NULL;
		}
    }

    // ------------------------------------------------------------------------
    // This puts data into the sending queue.
    // ------------------------------------------------------------------------
    void BufferData( const char* p_buffer, int p_size );

    // ------------------------------------------------------------------------
    // This sends the contents of the send buffer
    // ------------------------------------------------------------------------
    void SendBuffer();

    // ------------------------------------------------------------------------
    // This receives a buffer of data, and sends it off to the protocol to be
    // translated.
    // ------------------------------------------------------------------------
    void Receive();


    // ------------------------------------------------------------------------
    // This gets the receiving datarate for the socket, in bytes per
    // second, calculated over the previous interval
    // ------------------------------------------------------------------------
    inline int GetDataRate() const
    {
        return m_lastdatarate;
    }

    // ------------------------------------------------------------------------
    // This gets the current receiving datarate for the socket, in bytes per
    // second.
    // ------------------------------------------------------------------------
    inline int GetCurrentDataRate() const
    {
        return m_datarate / TIMECHUNK;
    }

    // ------------------------------------------------------------------------
    // Gets the amount of bytes within the sending buffer.
    // ------------------------------------------------------------------------
    inline int GetBufferedBytes() const
    {
        return (int)m_sendbuffer.size();
    }

    inline BasicLib::sint64 GetCreationTime() const
    {
        return m_creationtime;
    }

    inline Translator& GetTranslator()         { return *p_translator; }
    inline bool Closed()                { return m_closed; }

    inline void SwitchHandler( ConnectionHandler* p_handler )
    {
        if( Handler() )
        {
            Handler()->Leave();     // leave the current state if it exists
            delete Handler();       // delete state
            m_handlerstack.pop();   // pop the pointer off
        }

        m_handlerstack.push( p_handler );
        p_handler->Enter();     // enter the new state
    }

    inline void AddHandler( ConnectionHandler* p_handler )
    {
        if( Handler() )
            Handler()->Leave(); // leave the current state if it exists
        m_handlerstack.push( p_handler );
        p_handler->Enter();     // enter the new state		
    }

    inline void RemoveHandler()
    {
        Handler()->Leave();     // leave current state
        delete Handler();       // delete state
        m_handlerstack.pop();   // pop the pointer off
        if( Handler() )         // if old state exists, 
            Handler()->Enter(); // tell it connection has re-entered
    }


    inline ConnectionHandler* Handler()
    {
        if( m_handlerstack.size() == 0 )
            return 0;
        return m_handlerstack.top();
    }


    void ClearHandlers()
    {
        // leave the current handler
        if( Handler() )  
            Handler()->Leave();

        // delete all the handlers on the stack
        while( Handler() )
        {
            delete Handler();
            m_handlerstack.pop();
        }
    }

protected:


    // This is the protocol object that will manage data when it is received.
    Translator  * p_translator;

    // This stack manages the current data handler. When data is received,
    // it is sent off to the protocol policy object, which in turn sends it
    // to the handler on the top of this stack.
    std::stack< ConnectionHandler* > m_handlerstack;

    // This buffer stores all the data to be sent at a later time.
    std::string m_sendbuffer;

    // This is the amount of data received during the current chunk of time
    int m_datarate;

    // This is the amount of data that was received during the last chunk of
    // time
    int m_lastdatarate;

    // This represents the last time data was successfully received on the
    // connection, in seconds.
    BasicLib::sint64 m_lastReceiveTime;

    // The last time data was successfully sent.
    BasicLib::sint64 m_lastSendTime;

    // the time at which the connection was created; in seconds
    BasicLib::sint64 m_creationtime;

    // This boolean is used to determine if the send time should be checked 
    // when the GetLastSendTime function is invoked. When false, it is safe to 
    // assume that even if there is data in the queue, there have been no 
    // problems sending it thus far.
    bool m_checksendtime;

    char m_buffer[BUFFERSIZE];

    // determines whether or not the connection has been closed.
    bool m_closed;
};


}   // end namespace socketlib


#endif
