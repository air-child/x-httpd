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




int sock; //the server socket


int bAllowRemoteConnections = 0; //allow http connections from hosts other than localhost
int bRequirePassword = 1; //require http authentication.

int bLogDebugToConsole = 0;



