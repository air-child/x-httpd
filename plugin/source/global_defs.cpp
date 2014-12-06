/*

This software is made available under the GNU Affero GPL Version 3 license. 
Please see agpl.txt for more information.

Copyright 2005,2013 - Ben Russell, br@x-plugins.com

*/


//
//  global_defs.cpp
//  x-httpd
//
//  Created by Ben Russell on 20/11/13.
//
//

#include "global_defs.h"



//Used for magic IPC interchange.
char hack_blob[8192];



int sock; //the server socket


int bAllowRemoteConnections = 0; //allow http connections from hosts other than localhost
int bRequirePassword = 1; //require http authentication.

int bLogDebugToConsole = 0;






//write buffer pointers.
char *inbuf;
char *html;


unsigned char *auth_token_raw; //storage for our base64 encoded auth token.
unsigned char *auth_token_b64; //storage for our base64 encoded auth token.


unsigned char *generic_cache;



//std::map<intptr_t, client_handle> xhttpd_mapClientReturnPacketQueue;
std::map<std::string, std::string> xhttpd_mapResourceMap; //map: uri -> plugin_id

std::map<std::string, std::string> xhttpd_mapMimeTypes;
