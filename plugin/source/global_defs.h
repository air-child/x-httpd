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



#include "common_includes.h"



extern char hack_blob[8192];

extern int bAllowRemoteConnections; //allow http connections from hosts other than localhost

extern int bRequirePassword; //require http authentication.

extern int bLogDebugToConsole;



#endif
