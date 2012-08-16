// MUD Programming
// Ron Penton
// (C)2003
// Telnet.cpp - This is the header for the telnet policy class; it receives raw 
//              data from a connection and strips out or takes care of any 
//              command codes.


#include "Translator.h"
#include "../UserDB/UserDB.h"

namespace SocketLib
{

using std::string;

// ------------------------------------------------------------------------
//  Translates raw byte data into telnet data, and may send it out to
//  the connection's current protocol handler
// ------------------------------------------------------------------------

void Translator::Translate( Connection& p_conn, char* p_buffer, int p_size )
{	
	strRecvData.append(p_buffer, p_size);	

	if (strRecvData.length()>m_buffersize)
	{
		BasicLib::DeBugOut(UserDatabase::find( &p_conn )->name + " strRecvData too long");
		strRecvData.substr(strRecvData.length() - m_buffersize);
	}

	size_t nPos = strRecvData.find("\r\n");

	if (nPos != string::npos)
	{
		if (strRecvData.substr(0,4) == "send" )
		{
			if (strRecvData.size() - (nPos+strlen("\r\n")) < FileBlockLen)
			{
				return;
			}

			nPos +=strlen("\r\n") + FileBlockLen;
		}
		
		if (p_conn.Handler() != 0)
		{
			p_conn.Handler()->Handle( strRecvData.substr(0, nPos) );
			strRecvData = strRecvData.substr(nPos);

			if (strRecvData.substr(0,2) == "\r\n")
			{
				strRecvData = strRecvData.substr(2);
			}
		}
	}

}


// ------------------------------------------------------------------------
//  This sends a plain text string to the connection, doing any translations
//  if neccessary.
// ------------------------------------------------------------------------
void Translator::SendString( Connection& p_conn, string p_string )
{
    // just buffer the data on the connection
    p_conn.BufferData( p_string.data(), (int)p_string.size() );
}


}   // end namespace SocketLib
