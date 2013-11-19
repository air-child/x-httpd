/*

This software is made available under the GNU Affero GPL Version 3 license. 
Please see agpl.txt for more information.

Copyright 2005,2013 - Ben Russell, br@x-plugins.com

*/


//
//  global_defs.h
//  x-httpd
//
//  Created by Ben Russell on 19/11/13.
//
//

#ifndef x_httpd_global_defs_h
#define x_httpd_global_defs_h


//#define BUFSIZE 65536
#define BUFSIZE 4194304 //4 megs max

//too many defines 'cause once we had a read buffer and write buffer of differing sizes.
#define BUFFER_PAGE_SIZE BUFSIZE
#define INBUF_SIZE BUFSIZE



#define X_HTTPD_VERSION_STRING "x-httpd v0.1a (C) 2005,2013, br@x-plugins.com"





extern int sock; //the server socket


extern int bAllowRemoteConnections; //allow http connections from hosts other than localhost
extern int bRequirePassword; //require http authentication.

extern int bLogDebugToConsole;




#endif
