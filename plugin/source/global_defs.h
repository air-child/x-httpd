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



#define X_HTTPD_VERSION_STRING "x-httpd v14.12.06.1913 (C) 2005,2013-2014, br@x-plugins.com"

#include "common_includes.h"


extern char hack_blob[8192];



extern int sock; //the server socket


extern int bAllowRemoteConnections; //allow http connections from hosts other than localhost
extern int bRequirePassword; //require http authentication.

extern int bLogDebugToConsole;







//write buffer pointers.
extern char *inbuf;
extern char *html;


extern unsigned char *auth_token_raw; //storage for our base64 encoded auth token.
extern unsigned char *auth_token_b64; //storage for our base64 encoded auth token.


extern unsigned char *generic_cache;


#include <map>

//extern std::map<intptr_t, client_handle> xhttpd_mapClientReturnPacketQueue;
extern std::map<std::string, std::string> xhttpd_mapResourceMap; //map: uri -> plugin_id

extern std::map<std::string, std::string> xhttpd_mapMimeTypes;


#endif
