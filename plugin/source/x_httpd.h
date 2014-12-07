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



#define X_HTTPD_VERSION_STRING "x-httpd v14.12.06.1913 (C) 2005,2013-2014, br@x-plugins.com"






class x_httpd{
	private:
	
		//char hack_blob[8192]; //used to exchange data via X-Plane IPC, move to main_xpl.cpp
	
		int sock;
		
		int bAllowRemoteConnections;
		int bRequirePassword;
		
		int bLogDebugToConsole;
		
		char *inbuf;
		char *html;
		
		unsigned char *auth_token_raw; //storage for our base64 encoded auth token.
		unsigned char *auth_token_b64; //storage for our base64 encoded auth token.

		unsigned char *generic_cache;
	
	
		//extern std::map<intptr_t, client_handle> xhttpd_mapClientReturnPacketQueue;
		std::map<std::string, std::string> xhttpd_mapResourceMap; //map: uri -> plugin_id
		std::map<std::string, std::string> xhttpd_mapMimeTypes;


		
		char *queryStringV[1024]; //lookup table for query string values after 1st stage parsing.
		int queryStringVCount;
	
	
	public:
		x_httpd( int port );
		~x_httpd();


		void run(); //endless loop, run forever, CLI.
		void run_slice( int max_time_usec ); //run a small slice of work
		
		
		void initSockets();
		
		//handler functions for the web server.
		void processConnection( int client );





		void parseQuerystring( char *queryString );

		int parseQuerystringForInt( char *key );
		float parseQuerystringForFloat( char *key );
		void parseQuerystringForString( char *key, char *ret, int retSize );


		void header401Deny( char *header );

		void header200OK_MIME( char *header, const char* mime_string );

		void header404NF( char *header );





		int html404Document( char *header, char *html, char *requestString, char *queryString );
		int htmlAccessDenied( char *header, char *html );
		int htmlGeneric( char *header, char *html, char *payload );

		int htmlSendBinary( char *header, char *html, unsigned char *buffer, int size, char *fileType );

		//dataref access
		int htmlUniSet( char *header, char *html );
		int htmlUniGet( char *header, char *html );

		//summarised state data
		int htmlMiscStateXML( char *header, char *html, char *queryString );
		int htmlStateXML( char *header, char *html );





};



#endif /* defined(__x_httpd__http__) */
