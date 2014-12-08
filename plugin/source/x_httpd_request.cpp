//
//  x_httpd_request.cpp
//  x-httpd
//
//  Created by Ben on 7/12/2014.
//
//

#include "x_httpd_request.h"

#include "io_utils.h"


//extract auth token from raw data
void x_httpd_request::parseAuthToken(){
	
	//try and locate the authentication header.
	//std::string authorizationToken = rawData; //will be filled with b64 token
	this->authorizationToken = this->rawData;
	std::string::size_type headerLineStart = rawData.find("Authorization: Basic ");
	if( headerLineStart != std::string::npos ){
	
			//strips all bytes from start of request to the start of our B64 auth token.
			this->authorizationToken.replace(0,(int)headerLineStart+(strlen("Authorization: Basic ")), "");					
			
			std::string::size_type headerLineStop = this->authorizationToken.find("\n");
			size_t stop = this->authorizationToken.length() - headerLineStop; //location in raw bytes of end of auth line
			this->authorizationToken.replace( (int)headerLineStop, stop, "" ); //wtf?
			
			//replace trailing CR char if we find one
			if( this->authorizationToken[ authorizationToken.length()-1 ] == '\r' ){
				this->authorizationToken[ authorizationToken.length()-1 ] = 0;
			}
			
			printf("trimmed auth token (%s)\n", this->authorizationToken.c_str() );
			
		
	}else{
			//printf("No HTTP Auth token!\n");
			this->authorizationToken = "";
	
	}

} //parseAuthToken(...)



void x_httpd_request::decodeUrlEntities(){

	//FIXME: Entity decoding is a hack.
	//time to repair the request and query string data
	
	std::string fixUrlEntities = queryString;
	std::string::size_type startPos;
	//find / chars
	do{
			startPos = fixUrlEntities.find("%2F");
			if( (int)startPos >= 0 ){
				fixUrlEntities.replace( startPos, 3, "/" );
			}
	}while( (int)startPos >= 0 );
	

	//find [ chars
	do{
			startPos = fixUrlEntities.find("%5B");
			if( (int)startPos >= 0 ){
				fixUrlEntities.replace( startPos, 3, "[" );
			}
	}while( (int)startPos >= 0 );


	//find ] chars
	do{
			startPos = fixUrlEntities.find("%5D");
			if( (int)startPos >= 0 ){
				fixUrlEntities.replace( startPos, 3, "]" );
			}
	}while( (int)startPos >= 0 );
			

	//find < chars
	do{
			startPos = fixUrlEntities.find("<");
			if( (int)startPos >= 0 ){
				fixUrlEntities.replace( startPos, 1, "&lt;" );
			}
	}while( (int)startPos >= 0 );
	//find > chars
	do{
			startPos = fixUrlEntities.find(">");
			if( (int)startPos >= 0 ){
				fixUrlEntities.replace( startPos, 1, "&gt;" );
			}
	}while( (int)startPos >= 0 );
					
					

					
	//strcpy( queryString, fixUrlEntities.c_str() );
	this->queryString = fixUrlEntities;
	if( bLogDebugToConsole ){
		char caDbg[1024];
		sprintf( caDbg, "Decoded URL: (%s)\n", this->queryString.c_str() ); 
		printf( "%s", caDbg );
		//XPLMDebugString(caDbg);
	}

	//--end of repairs

}



void x_httpd_request::parseRequest(){

	char caDbg[1024];


	size_t iRequestStringStart = std::string::npos;
	size_t iRequestStringStop = std::string::npos;
	
	size_t iQueryStringStart = std::string::npos;
	size_t iQueryStringStop = std::string::npos;
	
	
		//Parse the request header for it's METHOD, Query String and protocol version.
		size_t iStringLen = this->rawData.size();
		for(size_t iX=0; iX<iStringLen; iX++){
			switch( this->rawData[iX] ){
				case '?':
					if( std::string::npos == iQueryStringStart ){
						iQueryStringStart = iX+1;
					}

				case ' ':
						{
							if( std::string::npos == iRequestStringStart ){
								iRequestStringStart = iX+1;
							}else{
								iRequestStringStop = iX;
								//request segment finished parsing, eject!
								iX=iStringLen;
							}						
						}
					break;
				case '\n':
				case '\r':
						iX = iStringLen;
					break;
				default:
					break;
			}
		}//end of for loop.
		
		
		
		
		
			//loop again, this time looking for the query string
				if( iQueryStringStart != std::string::npos ){
					for(size_t iX=iQueryStringStart; iX<iStringLen; iX++){
						switch( this->rawData[iX] ){
							case ' ':
									{
										if( std::string::npos == iQueryStringStop ){
											iQueryStringStop = iX;
											//request segment finished parsing, eject!
											iX=iStringLen;
										}						
									}
								break;
							case '\n':
							case '\r':
									iX = iStringLen;
								break;
							default:
								break;
						}
					}//end of for loop.
				}
			

		
		//copy raw request to a char blob for easier slicing with memcpy
		char caRawData[8192];
		strcpy( caRawData, this->rawData.c_str() );
		
		
		
		printf( "Parsed request(%li): RequestString: %li -> %li\n", iStringLen, iRequestStringStart, iRequestStringStop );
		if( iRequestStringStart != std::string::npos && iRequestStringStop != std::string::npos ){
			//memcpy( requestString, inbuf+iRequestStringStart, iRequestStringStop-iRequestStringStart );
			
			char caTmp[8192];
			memset( caTmp, 0, 8192 );
			memcpy( caTmp, caRawData + iRequestStringStart, iRequestStringStop - iRequestStringStart );

			this->requestString = std::string( caTmp );
			
			printf( "Extracted request string:(%s)\n", this->requestString.c_str() );
			
			//we now know which resource the client wants.
			
		}else{
			if( bLogDebugToConsole ){
				printf(caDbg, "IRStart/IRStop; %i / %i\n", iRequestStringStart, iRequestStringStop );
			}
		}




			//printf( "Parsed request(%i): QueryString: %i > %i\n", iStringLen, iRequestStringStart, iRequestStringStop );
			if( iQueryStringStart != std::string::npos && iQueryStringStop != std::string::npos ){
			
				//FIXME: memcpy
				char caTmp[8192];
				memset( caTmp, 0, 8192 );
				memcpy( caTmp, caRawData + iQueryStringStart, iQueryStringStop - iQueryStringStart );

				this->queryString = std::string( caTmp );
			
				if( bLogDebugToConsole ){
					sprintf(caDbg, "x-httpd request querystring: (%s)\n", this->queryString.c_str() ); 
					printf( "%s", caDbg );
					//XPLMDebugString(caDbg);
				}
				
				
			}else{
				if( bLogDebugToConsole ){
					sprintf( caDbg, "IQStart/IQStop; %li / %li\n", iQueryStringStart, iQueryStringStop ); 
					printf( "%s", caDbg );
					//XPLMDebugString(caDbg);
				}
				
				//the user has submitted an ivalid request and we could not parse the requested-file string out of it.
				
				//FIXME: Terminate connection
				
				//close( c );
				//return;
				
			}


} //x_httpd_request::parseRequest(...)






x_httpd_request::x_httpd_request( int sock_client, std::string sAuthTokenB64 ){

	this->bRequirePassword = 0;
	this->bLogDebugToConsole = 1;


	this->mapMimeTypes[".htm"] = "text/html";
	this->mapMimeTypes[".js"] = "text/plain"; //FIXME
	this->mapMimeTypes[".txt"] = "text/plain";
	this->mapMimeTypes[".css"] = "test/css";
	this->mapMimeTypes[".xml"] = "application/xml";
	this->mapMimeTypes[".png"] = "image/png";
	this->mapMimeTypes[".ico"] = "image/x-icon";
	this->mapMimeTypes[".swf"] = "application/x-shockwave-flash";
	this->mapMimeTypes[".bin"] = "application/octet-stream";


	//FIXME: Dynamic configuration required.
	sprintf( this->webRoot, "/Applications/X-Plane 10 beta/Resources/plugins/x-httpd.x-plugin/x-httpd-content/" );
	
	this->sAuthTokenB64 = sAuthTokenB64;

	this->queryStringVCount = 0;
	
	
		//setup socket and FILE* for read/write of socket
		this->sock_client = sock_client;
		this->sockIn = fdopen( this->sock_client, "rb" );
		this->sockOut = fdopen( this->sock_client, "wb" );
		this->response.setSocket( this->sockOut );
		this->response.setWebRoot( this->webRoot );


		//read all bytes the client has sent us
		this->readClientRequest();

		//socket data packet has been read, we should parse the new data.
		this->parseRequest();
		this->decodeUrlEntities();
		this->parseAuthToken();

		//parse the query string into a LUT of k/v pairs.
		this->parseQuerystring(); //sort the query string into a LUT
			
		
		//make choices about what content to serve
		this->processRequest();
		

}



x_httpd_request::~x_httpd_request(){
		
	fclose( this->sockOut );
	fclose( this->sockIn );

	close( this->sock_client );

}




void x_httpd_request::readClientRequest(){

	// ------ Socket Read Loop ----
	
	size_t bytes_read = 0; //total bytes read
	size_t chunk_size = 0; //size of last chunk that was read
	
	
	//temporary RX buffer, any read op must be <= this buffer size.
	char rxbuf[8192];
	memset( rxbuf, 0, 8192 ); //reset an oversize buffer with 0 bytes.
	
	
	this->rawData = ""; //erase the content storage.
	
	do{

		//attempts to read more than 1 byte at a time result in a blank http packet???
		chunk_size = fread( rxbuf, 1, 1, sockIn );
		bytes_read += chunk_size;

		//append byte-string to existing byte-string data
		this->rawData += std::string( rxbuf, chunk_size );
		
	}while( chunk_size > 0 );
	

	//DO NOT close socket or socket read/write FILE* handles until we're completely finished with request.
	//fclose( sockIn );
	//resources are released in destructor
	// --- End Socket Read Loop ---
	
	
	#if 1
	if( bLogDebugToConsole ){
		char caDbg[1024];

		sprintf(caDbg, "---recv x-httpd request, %i bytes---\n", (int) this->rawData.size() );
		printf( caDbg );
		//XPLMDebugString(caDbg);
		
			sprintf(caDbg, "%s", this->rawData.c_str() );
			printf( caDbg );
			//XPLMDebugString(caDbg);
			
		sprintf(caDbg, "--- end recv ---\n");
		printf( caDbg );
		//XPLMDebugString(caDbg);
	}
	#endif

}






void x_httpd_request::processRequest(){

	char caDbg[1024];

		
		sprintf(caDbg, "x-httpd request: (%s)\n", this->requestString.c_str() ); 
		printf( "%s", caDbg );

		
		int iPayloadSize = 0;

		if( 
			( authorizationToken == this->sAuthTokenB64 ) 
			|| ( ! bRequirePassword ) 
		){
		
			if( bLogDebugToConsole ){
				//sprintf( caDbg, "Authorization is good!\n"); XPLMDebugString(caDbg);
			}
		
				if( "/about" == this->requestString ){
					
					//this->response.setContentType( "text/plain" );
					this->response.setContentBody( "x-httpd 14.12.08.1650 alpha<br>built: " __DATE__ __TIME__ );
					this->response.write();
					

				}else{
					//we should look on the disk for this file.
					
					//EXPERIMENTAL
					//check to see if another plugin has registered to handle this resource..
					std::map<std::string, std::string>::iterator it = mapResourceMap.find( std::string(requestString) );
					
					if( it != mapResourceMap.end() ){
					
						this->processRequest_IPC();
					
					}else{
					
						//we could not find a registered filter above, so we'll now look for a static file on disk.
						
						//Default resource handler: http://localhost:1312/ -> http://localhost:1312/index.htm
						if( "/" == this->requestString ){
							this->requestString = "/index.htm";
						}
						
						
						this->response.sendFile( this->requestString.c_str() );
								
								
					} //end check for registered filter for uri

				} //end of checks for builts-ins, registered-filters and static-files
				
				
								

		}else{
			if( bLogDebugToConsole ){
				sprintf( caDbg, "HTTP Auth is BAD!\n"); 
				printf( "%s", caDbg );
				//XPLMDebugString( caDbg );

				sprintf( caDbg, "recv: (%s) wanted: (%s)\n", authorizationToken.c_str(), this->sAuthTokenB64.c_str() ); 
				printf( "%s", caDbg );
				//XPLMDebugString(caDbg);
			}
			
			this->response.accessDenied("no reason","no message");
			
		}

} //processRequest(..)







void x_httpd_request::processRequest_IPC(){

						#if 0

						std::string sMsg = "x-httpd: found mapped filter for resource: " + std::string(it->first) + " -> " + it->second + "\n";
						XPLMDebugString(sMsg.c_str());
					
						std::string sPluginID = it->second;
						
						XPLMPluginID target = XPLMFindPluginBySignature( sPluginID.c_str() ); //const char *         inSignature);
					
						if( target != XPLM_NO_PLUGIN_ID ){
							//we located the desired plugin, lets send it a message.
							long SEND_BLOB = 0x0100b10b;
							XPLMDebugString("x-httpd: sending ixplc blob..\n");
							
							//we need to wrap the input buffer in a very small header to make it blob safe.
							int packet_size = strlen(inbuf);;
							sprintf( caDbg, "  blob size: %i (%li)\n", packet_size, bytes_read);
							XPLMDebugString(caDbg);
							
							char* new_packet = (char*)malloc( packet_size+4 );
								memset( new_packet, 0, packet_size+4 );
								
								memcpy( new_packet, &packet_size, 4 );
								memcpy( new_packet+4, inbuf, packet_size );
							
								XPLMSendMessageToPlugin( target, SEND_BLOB, (void*)new_packet );
							
							XPLMDebugString("  erasing packet memory.\n");
							memset( new_packet, 0, packet_size+4 );
							free( new_packet );
							
							//data for this exchange will be returned inside the function: XPluginReceiveMessage
							//IF we rx an immediate response from the target plugin then the code below can run immediately.

						}
						
						
						//if we did not rx a reply from the filter plugin a 500 error will be written.
						
						//Code currently expects synchronous response.
						
							//Use a custom socket write handler for ixplc packets.
								//we have processed the request and gathered some kind of HTTP response.
								//time to write it to the client.
									sockOut = fdopen( c, "wb" );

										fwrite( hack_blob, strlen(hack_blob), 1, sockOut );

										fflush( sockOut );

									fclose( sockOut );

								close( c );
								
								
								//erase the xfer blob and re-write it with a 500 server error.
								//if the IPC code fails because a plugin does not answer (disabled/unloaded/etc)
								//the server will return a 500 server error packet for the requested item
								memset( hack_blob, 0, 8192 );
								sprintf( hack_blob, "HTTP/1.0 500 OK\r\n\r\n500: ixplc failed: timeout." );

						
						
						
						return;
						#endif

}






void x_httpd_request::parseQuerystring(){

/*
	memset(queryStringV, 0, sizeof(queryStringV));

	char **ap;
	int argc=0;
	
	for (ap = queryStringV; (*ap = strsep(&queryString, "&")) != NULL;){
		if (**ap != '\0'){
			argc++;
			if (++ap >= &queryStringV[256]){
				break;
			}
		}
	}

	this->queryStringVCount = argc;
*/
	
}


int x_httpd_request::parseQuerystringForInt( char *key ){
	char tmpret[32];
	parseQuerystringForString( key, tmpret, 32 );
	
	return atoi( tmpret );
}
float x_httpd_request::parseQuerystringForFloat( char *key ){
	char tmpret[32];
	parseQuerystringForString( key, tmpret, 32 );
	
	return atof( tmpret );
}


//This function is nasty.
void x_httpd_request::parseQuerystringForString( char *key, char *ret, int retSize ){

	char caDbg[1024];
	//sprintf( caDbg, "parsing query string for value of: (%s)\n", key); XPLMDebugString( caDbg );

	int x=0;
    char **ap, *argv[2];//, *kv;

	for(x=0; x<queryStringVCount; x++){
	
		//sprintf( caDbg, "dref LUT dump: %s\n", queryStringV[x]);  XPLMDebugString( caDbg );
	
			char kv[strlen(queryStringV[x])+1];// = queryStringV[x];
			strcpy(kv, queryStringV[x]);
			char *kvp = kv;

			for (ap = argv; (*ap = strsep(&kvp, "=")) != NULL;){
				if (**ap != '\0'){
					if (++ap >= &argv[2]){
						break;
					}
				}
			}
			
			//sprintf( caDbg, "parsing: (%s) = (%s)\n", argv[0], argv[1]);  XPLMDebugString( caDbg );
				
			if( strcmp( argv[0], key ) == 0 ){
				//sprintf( caDbg, "parser located: (%s) = (%s)\n", argv[0], argv[1]);  XPLMDebugString( caDbg );
				
				if( argv[1] != 0 ){
					if( (int)strlen(argv[1]) < retSize ){ //buff overflow protection
						strcpy( ret, argv[1] );
					}else{
						//XPLMDebugString("x-httpd: Error: query string parser: return value overflowed.");
						strcpy( ret, "overflow" );
					}
				}else{
					ret = "";
				}
			}
	
	
	
	}

}











//eof
