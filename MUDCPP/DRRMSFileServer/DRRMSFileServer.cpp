#define WIN32_LEAN_AND_MEAN
#include <stdio.h>

#include "../Lib/SocketLib/SocketLib.h"
#include "../Lib/ThreadLib/ThreadLib.h"

int main()
{
	
	
	
	SocketLib::ListeningManager lm;
	SocketLib::ConnectionManager cm( 60000 );
	
	lm.SetConnectionManager( &cm );
	lm.AddPort( 5099 );
	
	while( 1 )
	{
		lm.Listen();
		cm.Manage();
		ThreadLib::YieldThread();
	}
	
	return 0;
}
