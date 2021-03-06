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

#include <sstream>


#include "common_includes.h"

#include "x_httpd_response.h"

#include "timer/Timer.h"


//including this creates a pre-processor cycle which causes compilation failure.
//#include "x_httpd_responder.h"


class x_httpd_request{
	private:

		int sock_client; //raw socket.
		FILE *sockIn, *sockOut;

		std::string rawData; //collected by socket reader
		

		//GET /request/string?query=string HTTP/1.0
		//moving to public declaration so responder objects can access easily.
		/*
		std::string requestMode;
		std::string requestString;	
		std::vector<std::string> requestTokens;
		std::string queryString;	
		std::string httpVersion;
		*/
		
		std::map<std::string, std::string> map_Headers;
		
		std::string sBody; //body of request, form data, etc.


		std::string authorizationToken; //extracted from http request
		std::string sAuthTokenB64; //provided by parent


		

		int bLogDebugToConsole;
		int bRequirePassword;

		//we use a void* here because we cant inlucde the file that would give us the typedef or we get a cyclical error
		//the void* typedef is a placeholder for x_httpd_responder*
		std::map<std::string, void*> map_Responders; //map: uri -> plugin_id

		std::string sWebRoot; //folder path
		
		
		
		//crude lookup table for query string values after 1st stage parsing.
		//FIXME: replace with vector or map
		char *queryStringV[1024]; 
		int queryStringVCount;

		
		Timer hpt;
		
	
	public:
		x_httpd_request( int sock_client, std::string sAuthTokenB64 );
		~x_httpd_request();
		
		x_httpd_response response; //need public access in x_httpd_responder classes

		std::string requestMode;
		std::string requestString;	
		std::vector<std::string> requestTokens;
		std::string queryString;	
		std::string httpVersion;
		
		
		void setWebRoot( const char* folder );
		
		
		void readClientRequest(); //read from socket
		
		void parseRequest(); //decode basic HTTP request
		void decodeUrlEntities(); //filter URL entities
		void parseAuthToken(); //parse authentication token
		
		void parseQuerystring(); //break down query string
		int parseQuerystringForInt( char *key );
		float parseQuerystringForFloat( char *key );
		void parseQuerystringForString( char *key, char *ret, int retSize );

		void processRequest(); //default processcing
		void processRequest_IPC(); //advanced decode for IPC use with other plugins/modules.


};




#endif /* defined(__x_httpd__x_httpd_request__) */
