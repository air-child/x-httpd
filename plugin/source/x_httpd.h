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

#include "common_sdk.h"

#include "common_includes.h"




extern char *queryStringV[1024]; //lookup table for query string values after 1st stage parsing.
extern int queryStringVCount;

extern void parseQuerystring( char *queryString );

extern int parseQuerystringForInt( char *key );
extern float parseQuerystringForFloat( char *key );
extern void parseQuerystringForString( char *key, char *ret, int retSize );


extern void header401Deny( char *header );

extern void header200OK_MIME( char *header, const char* mime_string );

extern void header404NF( char *header );





extern int html404Document( char *header, char *html, char *requestString, char *queryString );
extern int htmlAccessDenied( char *header, char *html );
extern int htmlGeneric( char *header, char *html, char *payload );
//extern int htmlGenericText( char *header, char *html, char *payload );
//extern int htmlGenericXML( char *header, char *html, char *payload );
//extern int htmlGenericCSS( char *header, char *html, char *payload );
//extern int htmlSendImagePNG( char *header, char *html, unsigned char *buffer, int *size );

extern int htmlSendBinary( char *header, char *html, unsigned char *buffer, int size, char *fileType );

extern int htmlUniSet( char *header, char *html );

extern int htmlUniGet( char *header, char *html );


extern int htmlMiscStateXML( char *header, char *html, char *queryString );
extern int htmlStateXML( char *header, char *html );






extern int getFileSize( char *filename );


extern void cacheFile_Text( char *rootFolder, char *filename, unsigned char **buffer );
extern void cacheFile_Bin( char *rootFolder, char *filename, unsigned char **buffer, int *iSize );
extern void prepareFileCache();
extern void cleanupFileCache();


extern void findPluginFolder(char *buffer);
extern void findWebRoot( char *buffer );





//handler functions for the web server.
extern void processConnection( int client );




#endif /* defined(__x_httpd__http__) */
