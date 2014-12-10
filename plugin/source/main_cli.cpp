//
//  main.cpp
//  x-httpd-cli
//
//  Created by Ben on 7/12/2014.
//
//

#include <iostream>


#include "global_defs.h"

#include "x_httpd.h"


int main(int argc, const char * argv[])
{

	// insert code here...
	std::cout << XHTTPD_SERVER_MESSAGE << "\n";
	printf(" built: %s %s\n", __DATE__, __TIME__);
	
	
	
	
	
		char caDbg[1024];
	
		char caHostName[255];
		gethostname(caHostName, 255);
		
		struct hostent *host_entry;
		host_entry = gethostbyname( caHostName );
		
		char *caLocalIP;
		caLocalIP = inet_ntoa( *(struct in_addr *)*host_entry->h_addr_list);
		
		printf( "x-httpd: hostname: %s ip: %s\n", caHostName, caLocalIP );

	
	
	
	std::cout << "Spawning server..\n";
	x_httpd *httpd = new x_httpd( 1312 );
	httpd->run_cli(); //runs forever.
	
	
    return 0;
}

