#include "Connection.h"

namespace SocketLib
{
// ------------------------------------------------------------------------
// Constructors. One constructs a normal connection, and the other accepts
// a DataSocket to use as a "template" for the connection
// ------------------------------------------------------------------------

Connection::Connection()
{
    Initialize();
}


Connection::Connection( DataSocket& p_socket )
    : DataSocket( p_socket )
{
    Initialize();
}


void Connection::Initialize()
{
    m_datarate = 0;
    m_lastdatarate = 0;
    m_lastReceiveTime = 0;
    m_lastSendTime = 0;
    m_checksendtime = false;
    m_creationtime = BasicLib::GetTimeMS();
    m_closed = false;

	p_translator = new Translator();
}

// ------------------------------------------------------------------------
// This gets the amount of time that has passed, in seconds, since the
// last time data was successfully sent, if there is data still queued,
// or 0 if there is no data queued. This allows the server to check for
// deadlocked clients (ie: a client crashed and is not receiving data).
// ------------------------------------------------------------------------

BasicLib::sint64 Connection::GetLastSendTime() const
{
    if( m_checksendtime )
    {
        return BasicLib::GetTimeS() - m_lastSendTime;
    }

    return 0;
}


// ------------------------------------------------------------------------
// This puts data into the sending queue.
// ------------------------------------------------------------------------

void Connection::BufferData( const char* p_buffer, int p_size )
{
    m_sendbuffer.append( p_buffer, p_size );
}

// ------------------------------------------------------------------------
// This sends the contents of the send buffer
// ------------------------------------------------------------------------

void Connection::SendBuffer()
{
    if( m_sendbuffer.size() > 0 )
    {
        // send the data, and erase as much as was sent from the buffer.
        int sent = Send( m_sendbuffer.data(), (int)m_sendbuffer.size() );
        m_sendbuffer.erase( 0, sent );

		char szDebugOut[64];
		sprintf(szDebugOut,"sent %d chars", sent);
		BasicLib::DeBugOut(szDebugOut);

        if( sent > 0 )
        {
            // since data was sent successfully, reset the send time.
            m_lastSendTime = BasicLib::GetTimeS();
            m_checksendtime = false;
        }
        else
        {
            // no data was sent, so mark down that the sending time
            // needs to be checked, to see if the socket is entering
            // client deadlock
            if( !m_checksendtime )
            {
                // if execution gets here, that means that this connection
                // has previously not had any problems sending data, so
                // mark down the time that the sending problem started.
                // note that it may have started earlier, but since we didn't
                // start sending data earlier, there really is no way to know
                // for sure when it started.
                m_checksendtime = true;
                m_lastSendTime = BasicLib::GetTimeS();

            }
        }   // end no-data-sent check

    }   // end buffersize check
}

// ------------------------------------------------------------------------
// This receives a buffer of data and returns the number of bytes
// received. A return value of 0 is valid.
// ------------------------------------------------------------------------

void Connection::Receive()
{
    // receive the data
    int bytes = DataSocket::Receive( m_buffer, BUFFERSIZE );

    // get the current time
    BasicLib::sint64 t = BasicLib::GetTimeS();

    // check to see if we've reached the next X seconds of time (see Chapter 5)
    // and if so, clear the datarate.
    if( (m_lastReceiveTime / TIMECHUNK) != (t / TIMECHUNK) )
    {
        m_lastdatarate = m_datarate / TIMECHUNK;
        m_datarate = 0;
        m_lastReceiveTime = t;
    }

    m_datarate += bytes;

    // tell the protocol policy object about the received data.
	p_translator->Translate( *this, m_buffer, bytes );

}
}