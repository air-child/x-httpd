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
		
		std::map<std::string, std::string> mapMimeTypes;


		std::string sResponseType; //HTTP/1.0 200 OK, etc.
		std::string sContentType; //map into a header item???
		
		std::map<std::string, std::string> map_Headers;
		
		std::string sBody;

		

		FILE* sockOut; //FILE* socket output
		
		int bLogDebugToConsole;
		
		
		std::string sWebRoot; //root folder of static web content
		
		
		bool WriteBlock;

		
	public:
	
		x_httpd_response();
		~x_httpd_response();
		
		void setSocket( FILE* sockOut );
		
		void setWebRoot( const char* webRoot );
		
		//high level control of output
		
		//redirect to another URI
		void redirect( const char* target );
		
		//write a 401 access denied page with optional reason and message
		void accessDenied( const char* reason, const char* message );
		
		//write a 404
		void fileNotFound( const char* reason, const char* message );
		
		//write a 500 server error
		void serverError( const char* reason, const char* message );
		
		//write whatever is in the response buffers
		void write();
		
		
		void sendFile( const char* filename );
		
		
		
		//mid level control of output, general utils.
		void header200OK_MIME( char *header, const char* mime_string );
		
		int htmlGeneric( char *header, char *html, char *payload );
		int htmlSendBinary( char *header, char *html, unsigned char *buffer, int size, char *fileType );



		//low level control of output.
		void setResponse( const char *response ); //eg: HTTP/1.0 200 OK\n
		
		void setHeader( const char *key, const char *value ); //eg: Server: foo
		
		void setContentType( const char *mime_string ); //eg: application/json
		
		void setContentLength( size_t byte_count ); //eg: 12345
		
		void setContentBody( const char* content_body, size_t byte_count ); //eg: {"foo":123.45} - supports binary
		void setContentBody( const char* content_body ); //eg: {"foo":123.45} -- assumes C string
		
		


		//X-Plane specific content handlers...
		//FIXME: refactor
			//dataref access
			int htmlUniSet( char *header, char *html );
			int htmlUniGet( char *header, char *html );

			//summarised state data
			int htmlMiscStateXML( char *header, char *html, char *queryString );
			int htmlStateXML( char *header, char *html );


};




#endif /* defined(__x_httpd__x_httpd_response__) */
