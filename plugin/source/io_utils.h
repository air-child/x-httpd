//
//  io_utils.h
//  x-httpd
//
//  Created by Ben on 7/12/2014.
//
//

#ifndef __x_httpd__io_utils__
#define __x_httpd__io_utils__

#include <iostream>

#include "common_includes.h"






//server IO utility functions.
extern size_t getFileSize( const char *filename, char* error_message );

extern void cacheFile_Text( char *rootFolder, char *filename, unsigned char **buffer );
extern void cacheFile_Bin( char *rootFolder, char *filename, unsigned char **buffer, int *iSize );
extern void prepareFileCache();
extern void cleanupFileCache();


		//x-plane functions for the XPL to worry about..
extern void findPluginFolder(char *buffer);
extern void findWebRoot( char *buffer );




#endif /* defined(__x_httpd__io_utils__) */
