//
//  x_httpd_response.h
//  x-httpd
//
//  Created by Ben on 7/12/2014.
//
//

#ifndef __x_httpd__x_httpd_response__
#define __x_httpd__x_httpd_response__



#define X_HTTPD_VERSION_STRING "x-httpd v14.12.06.1913 (C) 2005,2013-2014, br@x-plugins.com"



#include <iostream>

#include <map>
#include <vector>
#include <string>

#include "common_includes.h"


class x_httpd_response{
	private:
		
		std::string sResponseType; //HTTP/1.0 200 OK, etc.
		
		std::map<std::string, std::string> map_Headers;
		
		char* response;
		std::string sBody;
		
		int bLogDebugToConsole;
		
		
		std::map<std::string, std::string> mapMimeTypes;

		
	public:
	
		x_httpd_response();
		~x_httpd_response();
	
		void header401Deny( char *header );
		void header200OK_MIME( char *header, const char* mime_string );
		void header404NF( char *header );
		
		
		int html404Document( char *header, char *html, const char *requestString, const char *queryString );
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




#endif /* defined(__x_httpd__x_httpd_response__) */
