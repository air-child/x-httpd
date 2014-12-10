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

#include "x_httpd_response.h"

#include "timer/Timer.h"


class x_httpd_request{
	private:

		int sock_client; //raw socket.
		FILE *sockIn, *sockOut;

		std::string rawData; //collected by socket reader
		

		//GET /request/string?query=string HTTP/1.0
		std::string requestMode;
		std::string requestString;	
		std::string queryString;	
		std::string httpVersion;
		
		std::map<std::string, std::string> map_Headers;
		
		std::string sBody; //body of request, form data, etc.


		std::string authorizationToken; //extracted from http request
		std::string sAuthTokenB64; //provided by parent


		x_httpd_response response;
		

		int bLogDebugToConsole;
		int bRequirePassword;

		std::map<std::string, std::string> mapResourceMap; //map: uri -> plugin_id

		std::string sWebRoot; //folder path
		
		
		
		//crude lookup table for query string values after 1st stage parsing.
		//FIXME: replace with vector or map
		char *queryStringV[1024]; 
		int queryStringVCount;

		
		Timer hpt;
		
	
	public:
		x_httpd_request( int sock_client, std::string sAuthTokenB64 );
		~x_httpd_request();
		
		
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
