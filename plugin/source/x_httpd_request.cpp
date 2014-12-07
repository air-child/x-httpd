//
//  x_httpd_request.cpp
//  x-httpd
//
//  Created by Ben on 7/12/2014.
//
//

#include "x_httpd_request.h"


//extract auth token from raw data
void x_httpd_request::parseAuthToken(){
	
	//try and locate the authentication header.
	//std::string authorizationToken = rawData; //will be filled with b64 token
	this->authorizationToken = this->rawData;
	std::string::size_type headerLineStart = rawData.find("Authorization: Basic ");
	if( (int)headerLineStart > -1 ){
	
			//strips all bytes from start of request to the start of our B64 auth token.
			this->authorizationToken.replace(0,(int)headerLineStart+(strlen("Authorization: Basic ")), "");					
			
			std::string::size_type headerLineStop = this->authorizationToken.find("\n");
			int stop = this->authorizationToken.length() - (int)headerLineStop; //location in raw bytes of end of auth line
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
		printf( caDbg );
		//XPLMDebugString(caDbg);
	}

	//--end of repairs

}



void x_httpd_request::parseRequest(){

	char caDbg[1024];


	int iRequestStringStart = -1;
	int iRequestStringStop = -1;
	
	int iQueryStringStart = -1;
	int iQueryStringStop = -1;
	
	
		//Parse the request header for it's METHOD, Query String and protocol version.
		int iX=0;
		int iStringLen = this->rawData.size();
		for(iX=0; iX<iStringLen; iX++){
			switch( this->rawData[iX] ){
				case '?':
					if( iQueryStringStart < 0 ){
						iQueryStringStart = iX+1;
					}

				case ' ':
						{
							if( iRequestStringStart == -1 ){
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
				if( iQueryStringStart > -1 ){
					for(iX=iQueryStringStart; iX<iStringLen; iX++){
						switch( this->rawData[iX] ){
							case ' ':
									{
										if( iQueryStringStop == -1 ){
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
		
		
		
		//printf( "Parsed request(%i): QueryString: %i > %i\n", iStringLen, iRequestStringStart, iRequestStringStop );
		if( iRequestStringStart > -1 && iRequestStringStop > -1 ){
			//memcpy( requestString, inbuf+iRequestStringStart, iRequestStringStop-iRequestStringStart );
			
			char caTmp[8192];
			memset( caTmp, 0, 8192 );
			memcpy( caTmp, caRawData + iRequestStringStart, iRequestStringStop - iRequestStringStart );

			this->requestString = std::string( caTmp );
			
			if( bLogDebugToConsole ){
				sprintf(caDbg, "x-httpd request: (%s)\n", this->requestString.c_str() ); 
				printf( caDbg );
				//XPLMDebugString(caDbg);
			}
			
		}else{
			if( bLogDebugToConsole ){
				//sprintf(caDbg, "IRStart/IRStop; %i / %i\n", iRequestStringStart, iRequestStringStop ); XPLMDebugString(caDbg);
			}
		}




			//printf( "Parsed request(%i): QueryString: %i > %i\n", iStringLen, iRequestStringStart, iRequestStringStop );
			if( iQueryStringStart > -1 && iQueryStringStop > -1 ){
			
				char caTmp[8192];
				memset( caTmp, 0, 8192 );
				memcpy( caTmp, caRawData + iQueryStringStart, iQueryStringStop - iQueryStringStart );

				this->queryString = std::string( caTmp );
			
				if( bLogDebugToConsole ){
					sprintf(caDbg, "x-httpd request querystring: (%s)\n", this->queryString.c_str() ); 
					printf( caDbg );
					//XPLMDebugString(caDbg);
				}
				
				
			}else{
				if( bLogDebugToConsole ){
					sprintf( caDbg, "IQStart/IQStop; %i / %i\n", iQueryStringStart, iQueryStringStop ); 
					printf( caDbg );
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
	sprintf( this->webRoot, "/Applications/X-Plane 10 beta/Resources/plugins/x-httpd.x-plugin/x-httpd-content/html/" );
	
	
	this->sock_client = sock_client;
	this->sAuthTokenB64 = sAuthTokenB64;



	this->queryStringVCount = 0;

	
	FILE *sockIn, *sockOut;

	char caDbg[1024];

		sockIn = fdopen( sock_client, "rb" );
			
			// ------ Socket Read Loop ----
			
			size_t bytes_read = 0;
			size_t chunk_size = 0;
			
			char rxbuf[8192];
			memset( rxbuf, 0, 8192 ); //reset an oversize buffer with 0 bytes.
			
			this->rawData = ""; //blankity blank blank
			
			do{
				//chunk_size = fread( inbuf + bytes_read, 1, 1, sockIn );
				chunk_size = fread( rxbuf, 1, 1, sockIn );
				bytes_read += chunk_size;

				//sprintf( caDbg, "  chunk read: %li bytes\n", chunk_size ); XPLMDebugString(caDbg);
				//usleep( 100 ); //do we need this?
				
				//append byte-string to existing byte-string data
				this->rawData += std::string( rxbuf, chunk_size );
				
			}while( chunk_size > 0 );
				
		//fclose( sockIn );
		
		
		//usleep( 1000 );
		//inbuf_bytes_read = fread( inbuf, INBUF_SIZE, 1, sockIn ); //read off the socket as fast as possible.
		//sprintf( caDbg, "  1st read: %li bytes\n", inbuf_bytes_read ); XPLMDebugString(caDbg);

		/*
		//if there is no data, we'll hang around trying to read until there is!
		int read_retries = 0;
		//while( inbuf_bytes_read < 10 && read_retries < 50 );{
		while( read_retries < 50 );{
			XPLMDebugString("####");
			usleep( 100 );
			size_t chunk_size = fread( inbuf, 8, 1, sockIn );
			inbuf_bytes_read += chunk_size;
			sprintf( caDbg, "x-httpd: chunk read: %li bytes\n", chunk_size ); XPLMDebugString(caDbg);
			sprintf( caDbg, "x-httpd: total read: %li bytes\n", inbuf_bytes_read ); XPLMDebugString(caDbg);

			read_retries++; //avoid DoS from open socket.
		}
		*/
				
		// --- End Socket Read Loop ---
		
		
		
		if( bLogDebugToConsole ){
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
			


		//socket data packet has been read, we should parse the new data.

		this->parseRequest();
		
		this->decodeUrlEntities();

		this->parseAuthToken();


		//parse the query string into a LUT of k/v pairs.
		this->parseQuerystring(); //sort the query string into a LUT
			




		//char response[BUFFER_PAGE_SIZE];
		//memset(response, 0, BUFFER_PAGE_SIZE);
		
		char header[2048];
		memset(header, 0, 2048);
		
		
		//FIXME: limited space for response data..
		char blob[8192];
		memset( blob, 0, 8192 );


#pragma mark Request Processor here.

		
		int iPayloadSize = 0;

		if( 
			( authorizationToken == this->sAuthTokenB64 ) 
			|| ( ! bRequirePassword ) 
		){
		
			if( bLogDebugToConsole ){
				//sprintf( caDbg, "Authorization is good!\n"); XPLMDebugString(caDbg);
			}
		
				if( "/about" == this->requestString ){
					//iPayloadSize = responseGeneric( header, response, (char *)page_index );
					iPayloadSize = this->response.htmlGeneric( header, blob, "x-httpd 14.12.06.1934 alpha" );

				//these items are dynamic
				/*
				}else if( strcmp( requestString, "/state.xml" ) == 0 ){
					iPayloadSize = responseStateXML( header, outbuf );

				}else if( strcmp( requestString, "/full-state.xml" ) == 0 ){
					iPayloadSize = responseMiscStateXML( header, outbuf, queryString ); //responseRootDocument( header, outbuf );
				*/
				
				/*
				}else if( "/get" == this->requestString ){
					iPayloadSize = this->htmlUniGet( header, outbuf );

				}else if( strcmp( requestString, "/set" ) == 0 ){
					iPayloadSize = this->htmlUniSet( header, outbuf );
				*/

				}else{
					//we should look on the disk for this file.
					
					//EXPERIMENTAL
					//check to see if another plugin has registered to handle this resource..
					std::map<std::string, std::string>::iterator it = mapResourceMap.find( std::string(requestString) );
					
					if( it != mapResourceMap.end() ){
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
					
					}else{
					
						//we could not find a registered filter above, so we'll now look for a static file on disk.
					
								
								//Default resource handler: http://localhost:1312/ -> http://localhost:1312/index.htm
								if( "/" == this->requestString ){
									this->requestString = "/index.htm";
								}
								
								
								
								//char webRoot[1024];
								//findWebRoot( webRoot );
								sprintf( caDbg, "webroot: %s\n", webRoot ); 
								//XPLMDebugString(caDbg);
								printf( caDbg );
								
								int iTempPayloadSize=0;
								
								char tmpFilename[2048];
								sprintf( tmpFilename, "response%s", this->requestString.c_str() );
								sprintf( caDbg, "Attempting to open and temp-cache the file:(%s)\n", tmpFilename ); 
								printf( caDbg );
								//XPLMDebugString(caDbg);
								
								
								//fixme; make the file-extension detector work with more than just three letter extensions.
								char tmpFileType[5];
								memset(	tmpFileType, 0, 5 );
								memcpy(
										tmpFileType,
										tmpFilename + (strlen(tmpFilename)-4), 
										4
										); //this should give us the file extension.
								
								printf( "File extension: %s\n", tmpFileType );
								
								
								//FIXME: check requested filename for ".." parent dir strings
								
								
								//todo; detect file type...
								//FIXME: Disabled for refactor efforts.
								/*
								cacheFile_Bin(webRoot, tmpFilename, &generic_cache, &iTempPayloadSize);
								
								if( iTempPayloadSize > 0 ){
									//sprintf( caDbg, "payload size returned by cacheFile_Bin: %i\n", iTempPayloadSize ); XPLMDebugString(caDbg);
								
									iPayloadSize = responseSendBinary(header, response, generic_cache, iTempPayloadSize, tmpFileType);
									//iPayloadSize = responseSendImagePNG( header, response, img_shadow, &img_shadow_size );
								
									free( generic_cache );
								}else{
									iPayloadSize = response404Document( header, response, requestString, queryString );
								}
								*/
								
					} //end check for registered filter for uri

				} //end of checks for builts-ins, registered-filters and static-files
				
				
				
				//double check content to see if we got a 404
				if( iPayloadSize == 0 ){
					printf("Failed to generate content. Defaulting to 404.\n");
					iPayloadSize = this->response.html404Document( header, blob, this->requestString.c_str(), this->queryString.c_str() );
				}

				

		}else{
			if( bLogDebugToConsole ){
				sprintf( caDbg, "HTTP Auth is BAD!\n"); 
				printf( caDbg );
				//XPLMDebugString( caDbg );

				sprintf( caDbg, "recv: (%s) wanted: (%s)\n", authorizationToken.c_str(), this->sAuthTokenB64.c_str() ); 
				printf( caDbg );
				//XPLMDebugString(caDbg);
			}
			iPayloadSize = this->response.htmlAccessDenied( header, blob );
			
		}





		//we have processed the request and gathered some kind of HTTP response.
		//time to write it to the client.
			sockOut = fdopen( this->sock_client, "w" );

				//write header
				fwrite( header, strlen(header), 1, sockOut );
					char tmp[256];
						sprintf(tmp, "Content-Length: %i\n\n", iPayloadSize);
							fwrite( tmp, strlen(tmp), 1, sockOut );
				
					//write payload
					fwrite( blob, iPayloadSize, 1, sockOut );

				fflush( sockOut );
		
			fclose( sockOut );
			fclose( sockIn );


		close( this->sock_client );


}



x_httpd_request::~x_httpd_request(){


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
