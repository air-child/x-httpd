/*

This software is made available under the GNU Affero GPL Version 3 license. 
Please see agpl.txt for more information.

Copyright 2005,2013 - Ben Russell, br@x-plugins.com

*/


//
//  http.h
//  x-httpd
//
//  Created by Ben Russell on 19/11/13.
//
//

#ifndef __x_httpd__http__
#define __x_httpd__http__

#include <iostream>
#include <map>
#include <string>
#include <vector>

//socket includes:


//project includes:
#include "io_utils.h"





class x_httpd{
	private:
		int sock;
		
		int bAllowRemoteConnections;
		int bRequirePassword;		
		int bLogDebugToConsole;
		
		std::string sAuthTokenRaw;
	
	public:
		x_httpd( int port );
		~x_httpd();

		void run(); //endless loop, run forever, CLI.
		void run_slice( int max_time_usec ); //run a small slice of work
		
		void initSockets();

};



#endif /* defined(__x_httpd__http__) */
