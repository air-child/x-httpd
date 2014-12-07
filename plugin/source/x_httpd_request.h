//
//  x_httpd_request.h
//  x-httpd
//
//  Created by Ben on 7/12/2014.
//
//

#ifndef __x_httpd__x_httpd_request__
#define __x_httpd__x_httpd_request__

#include <iostream>

#include <map>
#include <string>
#include <vector>



#include "common_includes.h"


#define X_HTTPD_VERSION_STRING "x-httpd v14.12.06.1913 (C) 2005,2013-2014, br@x-plugins.com"



class x_httpd_request{
	private:
		//GET /request/string?query=string HTTP/1.0
		std::string sMethod;	// GET
		std::string sRaw;	// /request/string?query=string
		std::string sRequestString; // /request/string
		std::string sQueryString;	// query=string
		
		std::map<std::string, std::string> map_Headers;
		
		std::string sBody;
		
		
		
		int sock_client;
		
		
		
		int bLogDebugToConsole;
		int bRequirePassword;
		
		//FIXME: need malloc resources.
		char* inbuf;		
		char* response;
		
		//refactor artifacts
		std::string sAuthTokenRaw;
		std::string sAuthTokenB64;

		unsigned char *generic_cache;
	
	
		//extern std::map<intptr_t, client_handle> xhttpd_mapClientReturnPacketQueue;
		std::map<std::string, std::string> xhttpd_mapResourceMap; //map: uri -> plugin_id
		std::map<std::string, std::string> xhttpd_mapMimeTypes;


		
		char *queryStringV[1024]; //lookup table for query string values after 1st stage parsing.
		int queryStringVCount;
	

		
		char webRoot[1024];
		
	
	public:
		x_httpd_request( int sock_client, std::string sAuthTokenB64 );
		~x_httpd_request();

		
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




#endif /* defined(__x_httpd__x_httpd_request__) */
