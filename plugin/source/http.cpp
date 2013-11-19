/*

This software is made available under the GNU Affero GPL Version 3 license. 
Please see agpl.txt for more information.

Copyright 2005,2013 - Ben Russell, br@x-plugins.com

*/


//
//  http.cpp
//  x-httpd
//
//  Created by Ben Russell on 19/11/13.
//
//

#include "http.h"


char *queryStringV[256]; //lookup table for query string values after 1st stage parsing.
int queryStringVCount = 0;




void header401Deny( char *header ){
	sprintf( header, "%s", 	
						"HTTP/1.0 401 Access Denied\n" 
						"Server: " X_HTTPD_VERSION_STRING "\n"
						"WWW-Authenticate: Basic realm=\"X-Plane\"\n"
						"Content-type: text/html\n"
						//"\n"
			);

}



void header200OK_HTM( char *header ){
	sprintf( header, "%s", 	
						"HTTP/1.0 200 OK\n"	
						"Server: " X_HTTPD_VERSION_STRING "\n"
						"Content-type: text/html\n"
						//"\n"
			);

}

void header200OK_CSS( char *header ){
	sprintf( header, "%s", 	
						"HTTP/1.0 200 OK\n"	
						"Server: " X_HTTPD_VERSION_STRING "\n"
						"Content-type: text/css\n"
						//"\n"
			);

}

void header200OK_TXT( char *header ){
	sprintf( header, "%s", 	
						"HTTP/1.0 200 OK\n"	
						"Server: " X_HTTPD_VERSION_STRING "\n"
						"Content-type: text/plain\n"
						//"\n"
			);

}

void header200OK_XML( char *header ){
	sprintf( header, "%s",
						"HTTP/1.0 200 OK\n"
						"Server: " X_HTTPD_VERSION_STRING "\n"
						"Content-Type: application/xml\n"
						//"\n"
			);
}


void header200OK_SWF( char *header ){
	sprintf( header, "%s",
						"HTTP/1.0 200 OK\n"
						"Server: " X_HTTPD_VERSION_STRING "\n"
						"Content-Type: application/x-shockwave-flash\n"
						//"\n"
			);
}


void header200OK_OctetStream( char *header ){
	sprintf( header, "%s",
						"HTTP/1.0 200 OK\n"
						"Server: " X_HTTPD_VERSION_STRING "\n"
						"Content-Type: application/octet-stream\n"
						//"\n"
			);
}


void header200OK_PNG( char *header ){
	sprintf( header, "%s",
						"HTTP/1.0 200 OK\n"
						"Server: " X_HTTPD_VERSION_STRING "\n"
						"Content-Type: image/png\n"
						//"\n"
			);
}
void header200OK_ICO( char *header ){
	sprintf( header, "%s", 
						"HTTP/1.0 200 OK\n"
						"Server: " X_HTTPD_VERSION_STRING "\n"
						"Content-Type: image/x-icon\n"
						//"\n"
			);
}


void header404NF( char *header ){
	sprintf( header, "%s", 
						"HTTP/1.0 404 Not Found\n"
						"Server: " X_HTTPD_VERSION_STRING "\n"
						"Content-Type: text/html\n"
						//"\n"
			);

}








int html404Document( char *header, char *html, char *requestString, char *queryString ){
	
	header404NF( header );

	int appVer, xplmVer;
	XPLMHostApplicationID appId;

	XPLMGetVersions( &appVer, &xplmVer, &appId );


	sprintf(html,	
					"<html><head><title>404 - File not found.</title></head>"
					"<body>\n"
					"\t<b>404 - File not found.</b><br/>\n"
//					"\tRequest(%i): (%s)<br/>\n"
					"\tQueryString(%i): (%s)<br/>\n"
					"<hr/>"
					"<font size='-1'>\n"
//					"\t" X_HTTPD_VERSION_STRING " - Compiled: " __DATE__ " " __TIME__ "<br/>\n"
					"\tX-Plane Version: %2.2f - "
						#if APL
							"Mac"
						#elif IBM
							"PC"
						#elif LIN
							"Linux"
						#endif					
						"<br/>\n"
					"XPLM Version: %2.2f<br/>\n"
					"</font>"
					"</body>"
					"</html>",
//						(int)strlen(requestString),
//						requestString,
						(int)strlen(queryString),
						queryString,
						appVer/100.0f,
						xplmVer/100.0f
					);

	return strlen( html );

}

int htmlAccessDenied( char *header, char *html ){

	header401Deny( header );
	sprintf( html, "%s", "Access Denied" );
	
	return strlen( html );

}



int htmlGeneric( char *header, char *html, char *payload ){

	header200OK_HTM( header );

	sprintf(html, "%s", 
						payload
						);
						
	return strlen( html );

}

/*

int htmlGenericText( char *header, char *html, char *payload ){

	header200OK_TXT( header );

	sprintf(html, "%s", 
						payload
						);
						
	return strlen( html );

}

int htmlGenericXML( char *header, char *html, char *payload ){

	header200OK_XML( header );

	sprintf(html, "%s", 
						payload
						);
						
	return strlen( html );

}
int htmlGenericCSS( char *header, char *html, char *payload ){

	header200OK_CSS( header );

	sprintf(html, "%s", 
						payload
						);
						
	return strlen( html );

}

int htmlSendImagePNG( char *header, char *html, unsigned char *buffer, int *size ){

	header200OK_PNG( header );
	
		memset( html, 0, *size+1 );
		memcpy( html, buffer, *size );

	return *size;

}
*/

int htmlSendBinary( char *header, char *html, unsigned char *buffer, int size, char *fileType ){


	if( strcmp( fileType, ".htm" ) == 0 ){
		header200OK_HTM( header );

	}else if( strcmp( fileType, ".txt" ) == 0 ){
		header200OK_TXT( header );

	}else if( strcmp( fileType, ".css" ) == 0 ){
		header200OK_CSS( header );

	}else if( strcmp( fileType, ".xml" ) == 0 ){
		header200OK_XML( header );

	}else if( strcmp( fileType, ".png" ) == 0 ){
		header200OK_PNG( header );
	
	}else if( strcmp( fileType, ".ico" ) == 0 ){
		header200OK_ICO( header );

	}else if( strcmp( fileType, ".swf" ) == 0 ){
		header200OK_SWF( header );
	
	}else{
		header200OK_OctetStream( header );
	
	}


		memset( html, 0, size );
		memcpy( html, buffer, size );

	return size;

}

int htmlUniSet( char *header, char *html ){
	header200OK_HTM( header );
	
	char drefName[1024];
	char drefType[4];
	
	char drefValueFormatString[64];
	
	
	parseQuerystringForString( "dref", drefName, 1024 );
	parseQuerystringForString( "type", drefType, 4 );
	
	XPLMDataRef tmpDr = XPLMFindDataRef( drefName );
	
	switch( drefType[0] ){
		case 'i':
			{
				int tmp = parseQuerystringForInt("val");
				XPLMSetDatai( tmpDr, tmp );
				sprintf( drefValueFormatString, "Int Value: (%i)<br/>\n", tmp );
			}
			break;
		case 'f':
			{
				float tmp = parseQuerystringForFloat("val");
				XPLMSetDataf( tmpDr, tmp );
				sprintf( drefValueFormatString, "Float Value: (%f)<br/>\n", tmp );
			}
			break;
		case 'c':
			{
				sprintf( drefValueFormatString, "String Value: (%s)<br/>\n", "foo" );
			}
			break;		
	}
	
	
	
	sprintf( html,
					//"Uni Set<br/>\n"
					"Dataref: (%s)<br/>\n"
					"Type: (%s)<br/>\n"
					"%s<br/>"
					
						//"<hr/>"
						//"<form action='/set' method='GET'>"
						//	"dref: <input type='text' name='dref' value='%s'/><br/>"
						//	"type: <input type='text' name='type' value='%s'/><br/>"
						//	"val: <input type='text' name='val'/></br>"
						//	"<input type='submit'/><br/>"
						//"</form>"
					,
						drefName,
						drefType,
							drefValueFormatString//,
						//drefName, //form field 1
						//drefType //form field 2
				);
					

	return strlen( html );

}

int htmlMiscStateXML( char *header, char *html, char *queryString ){



	//prepare GPS nav data.
	
	XPLMNavRef navRef = XPLMGetGPSDestination();
	XPLMNavType navType;// = XPLMGetGPSDestinationType();

	float gpsLat, gpsLon, gpsAlt;
	int gpsFreq;
	float gpsHeading;
	char gpsID[32];
	char gpsName[256];
	char gpsReg[256];

	XPLMGetNavAidInfo(	
						navRef,
						&navType,
						&gpsLat,
						&gpsLon,
						&gpsAlt,
						&gpsFreq,
						&gpsHeading,
						gpsID,
						gpsName,
						gpsReg
						);





	//this function fires as the root(/) url, it relies on /index.xsl for formatting.

		header200OK_XML( header );

		sprintf(html,
						"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n"
						//"<?xml-stylesheet type=\"text/xsl\" href=\"full-state.xsl\"?>\n"
						"<x-plane_full>\n"
						
							"<query-string value=\"%s\" />"
							
							"<aircraft>\n"

								"<position>\n"
									"<world lat=\"%2.5f\" lon=\"%3.5f\" alt_msl=\"%2.3f\" alt_agl=\"%2.3f\" />\n"
									"<opengl x=\"%2.5f\" y=\"%3.5f\" z=\"%2.3f\"/>\n"
									"<heading true=\"%2.3f\" mag=\"%2.3f\" track=\"%2.3f\" />\n"
									"<speed tas=\"%2.3f\" ias=\"%2.3f\" gs=\"%2.3f\" />\n"									
								"</position>\n"
								"<weather>\n"
									"<wind speed=\"%2.3f\" direction=\"%2.3f\" />\n"
								"</weather>\n"
								"<radios>\n"
									"<nav one=\"%i\" two=\"%i\" />\n"
									"<com one=\"%i\" two=\"%i\" />\n"
								"</radios>\n"
								//"<gps lat=\"%2.5f\" lon=\"%2.5f\" alt=\"%2.5f\" freq=\"%i\" hdg=\"%2.5f\" id=\"%s\" name=\"%s\" reg=\"%s\" />"
								"<gps lat=\"%2.5f\" lon=\"%2.5f\" alt=\"%2.5f\" id=\"%s\" name=\"%s\" />"
							
								"<cockpit>\n"
									"<buttons/>\n"
								"</cockpit>\n"
								
								"<failures/>\n"
								
							"</aircraft>\n"
							"<weather>\n"
								"<wind>\n"
									"<high "
										"alt=\"%1.3f\" "
										"speed=\"%1.3f\" "
										"dir=\"%1.3f\" "
										"sheer-speed=\"%1.3f\" "
										"sheer-direction=\"%1.3f\" "
										"turbulence=\"%1.3f\" />\n"
									"<med "
										"alt=\"%1.3f\" "
										"speed=\"%1.3f\" "
										"dir=\"%1.3f\" "
										"sheer-speed=\"%1.3f\" "
										"sheer-direction=\"%1.3f\" "
										"turbulence=\"%1.3f\" />\n"
									"<low "
										"alt=\"%1.3f\" "
										"speed=\"%1.3f\" "
										"dir=\"%1.3f\" "
										"sheer-speed=\"%1.3f\" "
										"sheer-direction=\"%1.3f\" "
										"turbulence=\"%1.3f\" />\n"
								"</wind>\n"
								"<clouds>\n"
									"\n"
								"</clouds>\n"
								"<misc>\n"
									"\n"
								"</misc>\n"
								"<sea_level_temp c=\"\" />\n"
								"<sea_level_pressure value=\"\" />\n"
							"</weather>\n"
						
						"</x-plane_full>\n"
						,
						queryString,
						//aircraft position and gps/radio state data
						XPLMGetDataf( xpdr_Lat ), XPLMGetDataf( xpdr_Lon ), XPLMGetDataf( xpdr_AltMSL ), XPLMGetDataf( xpdr_AltAGL ),	//world
						XPLMGetDataf( xpdr_X ), XPLMGetDataf( xpdr_Y ), XPLMGetDataf( xpdr_Z ),	//local
						XPLMGetDataf( xpdr_Heading ), XPLMGetDataf( xpdr_HeadingMag ), XPLMGetDataf( xpdr_HeadingTrack ),
						XPLMGetDataf( xpdr_Kias ), XPLMGetDataf( xpdr_Ktas ), XPLMGetDataf( xpdr_Kgs ),
						
						XPLMGetDataf( xpdr_WindSpeed ), XPLMGetDataf( xpdr_WindDirection ),
						
						XPLMGetDatai( xpdr_Nav1 ), XPLMGetDatai( xpdr_Nav1s ),
						XPLMGetDatai( xpdr_Com1 ), XPLMGetDatai( xpdr_Com1s ),
						
						//gpsLat, gpsLon, gpsAlt, gpsFreq, gpsHeading, gpsID, gpsName, gpsReg
						gpsLat, gpsLon, gpsAlt, gpsID, gpsName,

						
						
						//weather wind data
						XPLMGetDataf( xpdr_windAltHigh ), XPLMGetDataf( xpdr_windSpeedHigh ), XPLMGetDataf( xpdr_windDirHigh ), XPLMGetDataf( xpdr_windSheerSpeedHigh ), XPLMGetDataf( xpdr_windSheerDirectionHigh ), XPLMGetDataf( xpdr_windTurbulenceHigh ), 
						XPLMGetDataf( xpdr_windAltMed ), XPLMGetDataf( xpdr_windSpeedMed ), XPLMGetDataf( xpdr_windDirMed ), XPLMGetDataf( xpdr_windSheerSpeedMed ), XPLMGetDataf( xpdr_windSheerDirectionMed ), XPLMGetDataf( xpdr_windTurbulenceMed ), 
						XPLMGetDataf( xpdr_windAltLow ), XPLMGetDataf( xpdr_windSpeedLow ), XPLMGetDataf( xpdr_windDirLow ), XPLMGetDataf( xpdr_windSheerSpeedLow ), XPLMGetDataf( xpdr_windSheerDirectionLow ), XPLMGetDataf( xpdr_windTurbulenceLow )

						
						);
						
						
	return strlen( html );

}

int htmlStateXML( char *header, char *html ){

	//prepare GPS nav data.
	
	XPLMNavRef navRef = XPLMGetGPSDestination();
	XPLMNavType navType;// = XPLMGetGPSDestinationType();

	float gpsLat, gpsLon, gpsAlt;
	int gpsFreq;
	float gpsHeading;
	char gpsID[32];
	char gpsName[256];
	char gpsReg[256];

	XPLMGetNavAidInfo(	
						navRef,
						&navType,
						&gpsLat,
						&gpsLon,
						&gpsAlt,
						&gpsFreq,
						&gpsHeading,
						gpsID,
						gpsName,
						gpsReg
						);





		header200OK_XML( header );

		sprintf(html,
						"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n"
						//"<?xml-stylesheet type=\"text/xsl\" href=\"state.xsl\"?>\n"
						"<x-plane>\n"
							"<aircraft>\n"
								"<position>\n"
									"<world lat=\"%2.5f\" lon=\"%3.5f\" alt_msl=\"%2.3f\" alt_agl=\"%2.3f\" />\n"
									"<opengl x=\"%2.5f\" y=\"%3.5f\" z=\"%2.3f\"/>\n"
									"<heading true=\"%2.3f\" mag=\"%2.3f\" track=\"%2.3f\" />\n"
									"<speed tas=\"%2.3f\" ias=\"%2.3f\" gs=\"%2.3f\" />\n"									
								"</position>\n"
								"<weather>\n"
									"<wind speed=\"%2.3f\" direction=\"%2.3f\" />\n"
								"</weather>\n"
								"<radios>\n"
									"<nav one=\"%i\" two=\"%i\" />\n"
									"<com one=\"%i\" two=\"%i\" />\n"
								"</radios>\n"
								//"<gps lat=\"%2.5f\" lon=\"%2.5f\" alt=\"%2.5f\" freq=\"%i\" hdg=\"%2.5f\" id=\"%s\" name=\"%s\" reg=\"%s\" />"
								"<gps lat=\"%2.5f\" lon=\"%2.5f\" alt=\"%2.5f\" id=\"%s\" name=\"%s\" />"
							"</aircraft>\n"
							"<traffic>\n"
								
							"</traffic>\n"
						"</x-plane>\n",
						XPLMGetDataf( xpdr_Lat ), XPLMGetDataf( xpdr_Lon ), XPLMGetDataf( xpdr_AltMSL ), XPLMGetDataf( xpdr_AltAGL ),	//world
						XPLMGetDataf( xpdr_X ), XPLMGetDataf( xpdr_Y ), XPLMGetDataf( xpdr_Z ),	//local
						XPLMGetDataf( xpdr_Heading ), XPLMGetDataf( xpdr_HeadingMag ), XPLMGetDataf( xpdr_HeadingTrack ),
						XPLMGetDataf( xpdr_Kias ), XPLMGetDataf( xpdr_Ktas ), XPLMGetDataf( xpdr_Kgs ),
						
						XPLMGetDataf( xpdr_WindSpeed ), XPLMGetDataf( xpdr_WindDirection ),
						
						XPLMGetDatai( xpdr_Nav1 ), XPLMGetDatai( xpdr_Nav1s ),
						XPLMGetDatai( xpdr_Com1 ), XPLMGetDatai( xpdr_Com1s ),
						
						//gpsLat, gpsLon, gpsAlt, gpsFreq, gpsHeading, gpsID, gpsName, gpsReg
						gpsLat, gpsLon, gpsAlt, gpsID, gpsName
						
						);
						
						
	return strlen( html );

}






void parseQuerystring( char *queryString ){

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

	queryStringVCount = argc;

	
}


int parseQuerystringForInt( char *key ){
	char tmpret[32];
	parseQuerystringForString( key, tmpret, 32 );
	
	return atoi( tmpret );
}
float parseQuerystringForFloat( char *key ){
	char tmpret[32];
	parseQuerystringForString( key, tmpret, 32 );
	
	return atof( tmpret );
}
void parseQuerystringForString( char *key, char *ret, int retSize ){

	//printf("parsing query string for value of: (%s)\n", key);

	int x=0;
    char **ap, *argv[2];//, *kv;

	for(x=0; x<queryStringVCount; x++){
	
		//printf("dref LUT dump: %s\n", queryStringV[x]);
	
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
			
			//printf("parsing: (%s) = (%s)\n", argv[0], argv[1]);
				
			if( strcmp( argv[0], key ) == 0 ){
				//printf("parser located: (%s) = (%s)\n", argv[0], argv[1]);
				
				if( argv[1] != 0 ){
					if( (int)strlen(argv[1]) < retSize ){ //buff overflow protection
						strcpy( ret, argv[1] );
					}else{
						strcpy( ret, "" );
					}
				}else{
					ret = "";
				}
			}
	
	
	
	}

}










void processConnection( int c ){

	FILE *sockIn, *sockOut;

	

		sockIn = fdopen( c, "r" );
			
			//**this has been replaced with a global char* that is malloced.
			//char inbuf[INBUF_SIZE];
			
			memset(inbuf, 0, INBUF_SIZE);

			fread( inbuf, INBUF_SIZE, 1, sockIn ); //read off the socket as fast as possible.
			
			//if there is no data, we'll hang around trying to read until there is!
			int read_retries = 0;
			do{
				usleep( 1000 );
				fread( inbuf, INBUF_SIZE, 1, sockIn );
				read_retries++; //avoid DoS from open socket.
			}while( strlen( inbuf ) < 1 && read_retries < 50 );
			
		
			if( bLogDebugToConsole ){
				printf("---recv x-httpd request, %i bytes---\n", (int)strlen(inbuf));
					printf( "%s", inbuf );
				printf("--- end recv ---\n");
			}
			

		int iRequestStringStart = -1;
		int iRequestStringStop = -1;
		
		int iQueryStringStart = -1;
		int iQueryStringStop = -1;
		
		char requestMode[8];
			memset(requestMode, 0, sizeof(requestMode));
		char requestString[2048];
			memset(requestString, 0, sizeof(requestString));
		char queryString[2048];
			memset(queryString, 0, sizeof(requestString));
		char httpVersion[16];
			memset(httpVersion, 0, sizeof(httpVersion));



		int iX=0;
		int iStringLen = strlen( inbuf );
		for(iX=0; iX<iStringLen; iX++){
			switch( inbuf[iX] ){
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
						switch( inbuf[iX] ){
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
			

		
		//printf( "Parsed request(%i): QueryString: %i > %i\n", iStringLen, iRequestStringStart, iRequestStringStop );
		if( iRequestStringStart > -1 && iRequestStringStop > -1 ){
			memcpy( requestString, inbuf+iRequestStringStart, iRequestStringStop-iRequestStringStart );
			if( bLogDebugToConsole ){
				printf( "x-httpd request: (%s)\n", requestString );
			}
			
		}else{
			if( bLogDebugToConsole ){
				printf( "IRStart/IRStop; %i / %i\n", iRequestStringStart, iRequestStringStop );
			}
		}


			//printf( "Parsed request(%i): QueryString: %i > %i\n", iStringLen, iRequestStringStart, iRequestStringStop );
			if( iQueryStringStart > -1 && iQueryStringStop > -1 ){
				memcpy( queryString, inbuf+iQueryStringStart, iQueryStringStop-iQueryStringStart );
				if( bLogDebugToConsole ){
					printf( "x-httpd request querystring: (%s)\n", queryString );
				}
				
				
			}else{
				if( bLogDebugToConsole ){
					printf( "IQStart/IQStop; %i / %i\n", iQueryStringStart, iQueryStringStop );
				}
				//the user has submitted an ivalid request and we could not parse the requested-file string out of it.
				//close( c );
				//return;
			}



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
							
							

							
			strcpy( queryString, fixUrlEntities.c_str() );
			if( bLogDebugToConsole ){
				printf("Decoded URL: (%s)\n", queryString );
			}

			//--end of repairs



			//try and locate the authentication header.
			std::string rawData = inbuf;
			std::string authorizationToken = rawData; //will be filled with b64 token
			std::string::size_type headerLineStart = rawData.find("Authorization: Basic ");
			if( (int)headerLineStart > -1 ){
			
					authorizationToken.replace(0,(int)headerLineStart+(strlen("Authorization: Basic ")), "");					
					//printf("begin auth token: %s\n", authorizationToken.c_str());
			
					std::string::size_type headerLineStop = authorizationToken.find("\n");
					int stop = authorizationToken.length() - (int)headerLineStop;
					authorizationToken.replace( (int)headerLineStop, stop, "" );
					
					if( authorizationToken[ authorizationToken.length()-1 ] == '\r' ){
						authorizationToken[ authorizationToken.length()-1 ] = 0;
					}
					
					//printf("trimmed auth token (%s)\n", authorizationToken.c_str() );
					
				
			}else{
					//printf("No HTTP Auth token!\n");
					authorizationToken = "";
			
			}









			//parse the query string into a LUT of k/v pairs.
				char qsBufferCopy[ strlen(queryString)+1 ];
				strcpy( qsBufferCopy, queryString );
					parseQuerystring( qsBufferCopy ); //sort the query string into a LUT
					



		//char html[BUFFER_PAGE_SIZE];
		memset(html, 0, BUFFER_PAGE_SIZE);
		
		char header[2048];
		memset(header, 0, 2048);


#pragma mark Request Processor here.

		
		int iPayloadSize = 0;

		if( (strcmp(authorizationToken.c_str(), (const char *)auth_token_b64) == 0) || ( ! bRequirePassword ) ){
			if( bLogDebugToConsole ){
				printf("Authorization is good!\n");
			}
		
				if( strcmp( requestString, "/" ) == 0 ){
					//iPayloadSize = htmlGeneric( header, html, (char *)page_index );
					iPayloadSize = htmlGeneric( header, html, "x-httpd 13.11.20.0015 alpha" );



				//these items are dynamic
				}else if( strcmp( requestString, "/state.xml" ) == 0 ){
					iPayloadSize = htmlStateXML( header, html );

				}else if( strcmp( requestString, "/full-state.xml" ) == 0 ){
					iPayloadSize = htmlMiscStateXML( header, html, queryString ); //htmlRootDocument( header, html );

				}else if( strcmp( requestString, "/set" ) == 0 ){
					iPayloadSize = htmlUniSet( header, html );




				}else{
					//we should look on the disk for this file.
					
					char webRoot[1024];
					findWebRoot( webRoot );
					printf( "webroot: %s\n", webRoot );
					
					int iTempPayloadSize=0;
					
					char tmpFilename[2048];
					sprintf( tmpFilename, "html%s", requestString );
					
					printf("Attempting to open and temp-cache the file: %s\n", tmpFilename );
					
					
					//fixme; make the file-extension detector work with more than just three letter extensions.
					char tmpFileType[5];
					memset(	tmpFileType, 0, 5 );
					memcpy(
							tmpFileType,
							tmpFilename + (strlen(tmpFilename)-4), 
							4
							); //this should give us the file extension.
					
					printf("File extension: %s\n", tmpFileType );
					
					
					
					//fixme; check requested filename for ".." parent dir strings
					
					//todo; detect file type...
					cacheFile_Bin(webRoot, tmpFilename, &generic_cache, &iTempPayloadSize);
					
					if( iTempPayloadSize > 0 ){
						printf("payload size returned by cacheFile_Bin: %i\n", iTempPayloadSize );
					
						iPayloadSize = htmlSendBinary(header, html, generic_cache, iTempPayloadSize, tmpFileType);
						//iPayloadSize = htmlSendImagePNG( header, html, img_shadow, &img_shadow_size );
					
						free( generic_cache );
					}else{
						iPayloadSize = html404Document( header, html, requestString, queryString );
					}

				}
				
				
				
				//double check content to see if we got a 404
				if( iPayloadSize == 0 ){
					iPayloadSize = html404Document( header, html, requestString, queryString );
				}
				

		}else{
			if( bLogDebugToConsole ){
				printf("HTTP Auth is BAD!\n");
			}
			if( bLogDebugToConsole ){ 
				printf("recv: (%s) wanted: (%s)\n", authorizationToken.c_str(), (const char *)auth_token_b64);
			}
			iPayloadSize = htmlAccessDenied( header, html );
			
		}



		//we have processed the request and gathered some kind of HTTP response.
		//time to write it to the client.
			sockOut = fdopen( c, "w" );

				//write header
				fwrite( header, strlen(header), 1, sockOut );
					char tmp[256];
						sprintf(tmp, "Content-Length: %i\n\n", iPayloadSize);
							fwrite( tmp, strlen(tmp), 1, sockOut );
				
					//write payload
					fwrite( html, iPayloadSize, 1, sockOut );

				fflush( sockOut );
		

			fclose( sockIn );
			fclose( sockOut );

		close( c );



}//end processConnection()










void cacheFile_Text( char *rootFolder, char *filename, unsigned char **buffer ){

	char targetFile[ strlen(rootFolder) + strlen(filename) + 2 ];

	sprintf( targetFile, "%s%s", rootFolder, filename );
		printf( "attempting to cache text file: %s\n", targetFile );


	int iSize = getFileSize( targetFile ) + 1; //we add 1 to the end to ensure we have a terminator byte
		//if( iSize > 0 ){
			*buffer	= (unsigned char*)malloc( iSize );
				memset(*buffer, 0, iSize );
					
					FILE *fp = fopen( targetFile, "r" );
					if( fp ){
							fread( *buffer, iSize, 1, fp );
						fclose( fp );
						
						//printf( "file checksum: %i\n", checksumEnvelope( (char *)*buffer, iSize ) );

					}
		//}else{
		//	*buffer = (unsigned char*)malloc( 64 );
		//	sprintf( (char*)*buffer, "File not found." );
		
		//}

}
void cacheFile_Bin( char *rootFolder, char *filename, unsigned char **buffer, int *iSize ){

	char targetFile[ strlen(rootFolder) + strlen(filename) + 2 ];

	sprintf( targetFile, "%s%s", rootFolder, filename );
		printf( "attempting to cache binary file: %s\n", targetFile );

	*iSize = getFileSize( targetFile );
	
		if( *iSize > 0 ){
			*buffer = (unsigned char*)malloc( *iSize );
			memset(*buffer, 0, *iSize );
				
				FILE *fp = fopen( targetFile, "r" );
				if( fp != NULL ){
					//fixme: reading all in one massive chunk not so great for speed.
					fread( *buffer, *iSize, 1, fp );
					fclose( fp );
				}
		}


}


void prepareFileCache(){


	//global read buffer
	inbuf = (char *)malloc( BUFSIZE );
	html = (char *)malloc( BUFSIZE );



	char webRoot[1024];
	findWebRoot( webRoot );
	
	printf( "webroot: %s\n", webRoot );
	
		//cache authentication details
		cacheFile_Text( webRoot, "password.txt", &auth_token_raw );
				int b64_len = ((strlen((const char*)auth_token_raw))*2); //we multiply the buffer size by 2 to make sure we have room for the b64 version.
				auth_token_b64 = (unsigned char*)malloc( b64_len );
				memset(auth_token_b64, 0, b64_len );
					encode_string( (char *)auth_token_raw, (char *)auth_token_b64 );
					printf("Authentication token loaded and encoded: (%s) > (%s)\n", auth_token_raw, auth_token_b64 );

}


void cleanupFileCache(){

	//free the io buffers.
	free( inbuf ); //file reading buffer should be released immediately after file reads.
	free( html );


	free(auth_token_raw);
	free(auth_token_b64);

}


int getFileSize( char *filename ){

	struct stat fsize;
	
	if( !stat(filename, &fsize) ){
		return fsize.st_size;
	}else{
		printf("No such file (stat error): %s\n", filename);
	}
	
	return -1;

}








