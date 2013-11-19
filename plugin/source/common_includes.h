/*

This software is made available under the GNU Affero GPL Version 3 license. 
Please see agpl.txt for more information.

Copyright 2005,2013 - Ben Russell, br@x-plugins.com

*/


//
//  common_includes.h
//  x-httpd
//
//  Created by Ben Russell on 20/11/13.
//
//

#ifndef x_httpd_common_includes_h
#define x_httpd_common_includes_h



//winsock
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include <netdb.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>

#include <string.h>

#include <stdlib.h>



#pragma mark --



//usefull STL classes
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>





#include "b64.h"


#include "global_defs.h"
#include "http.h"

#include "drefs.h"





#endif
