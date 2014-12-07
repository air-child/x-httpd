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

#include "x_httpd.h"

#include "common_includes.h"



x_httpd::x_httpd( int port ){

	this->queryStringVCount = 0;
	
	//10 meg buffer.
	#define BUFSIZE 1024*1024*10
	this->inbuf = (char*)malloc( BUFSIZE );
	this->html = (char*)malloc( BUFSIZE );



	this->xhttpd_mapMimeTypes[".htm"] = "text/html";
	this->xhttpd_mapMimeTypes[".js"] = "text/plain"; //FIXME
	this->xhttpd_mapMimeTypes[".txt"] = "text/plain";
	this->xhttpd_mapMimeTypes[".css"] = "test/css";
	this->xhttpd_mapMimeTypes[".xml"] = "application/xml";
	this->xhttpd_mapMimeTypes[".png"] = "image/png";
	this->xhttpd_mapMimeTypes[".ico"] = "image/x-icon";
	this->xhttpd_mapMimeTypes[".swf"] = "application/x-shockwave-flash";

	this->xhttpd_mapMimeTypes[".bin"] = "application/octet-stream";



}

x_httpd::~x_httpd(){
	free( this->inbuf );
	free( this->html );
	
	//We'll do this another way..
	//free( this->auth_token_raw );
	//free( this->auth_token_b64 );
}





/**
Set the root content folder.
*/
/*
void x_httpd::setRootFolder( const char* target_folder ){

	strcpy(this->content_folder, target_folder);

}
*/


void x_httpd::run(){

	for(;;){
	
		this->run_slice( 10 );
		
		//we should probably usleep or something
		usleep( 10 );
	
	}
}



void x_httpd::run_slice( int time_usec ){

	//get time start
	//loop over work
		//do single work item
		//check time delta
		//if time delta > time_usec allowance, return
	//loop over more work
	





		char caDbg[1024];


			static int socketsInit = 0;
			if( ! socketsInit ){
			
				if( this->sAuthTokenRaw == "username:password" ){
					//XPLMSpeakString("x-httpd; This is the first time you have used x-httpd, please change the password.");
					//dialog_ChangePassword();
					
					printf("*** Password Needs Changing ***\n");
				
				}
				
				printf("Running sockets init...\n");
				this->initSockets();
				socketsInit = 1;
			
			}


			int c = 0; //client socket, populated by accept(...)
			int clientCount = 0;
			do{
				clientCount++;
			
				if( clientCount >= 10 ){ break; } //we want to give x-plane priority, 10 clients per frame event.
			
				struct sockaddr_in from;
				memset( &from, 0, sizeof( sockaddr_in ));
				socklen_t len;

				c = accept( this->sock, (sockaddr*)&from, &len );				
				
				switch( c ){
					case EBADF:
							printf("x-httpd error: Bad file descriptor.\n");
						break;
					case ENOTSOCK:
							printf("x-httpd error: accept() error: Not a socket.\n");
						break;
					case EOPNOTSUPP:
							printf("x-httpd error: accept() error: Socket is not a STREAM.\n");
						break;
					case EFAULT:
							printf("x-httpd error: accept() error: Address parameter fault.\n");
						break;
					case EWOULDBLOCK:
							printf( "x-httpd error: accept() error: Would block.\n" );
						break;
					case EMFILE:
							printf( "x-httpd error: accept() error: Process file table is full.\n" );
						break;
					case ENFILE:
							printf( "x-httpd error: accept() error: Would block.\n" );
						break;
					case -1:
							//This error tends to repeat a lot.
							//printf( "x-httpd error: accept() error: -1, see errono.\n" );
							
						break;
					default:
						printf("accept()ed a connect: %i\n", c);
						
						char remoteAddress[32];
							strcpy( remoteAddress, inet_ntoa(from.sin_addr) );
							
							sprintf( caDbg, "\nx-httpd: request from: %s\n", remoteAddress );
							printf( caDbg );
							//XPLMDebugString(caDbg);
							
							
							//if(true){
							if( (strcmp( "127.0.0.1", remoteAddress ) == 0) || bAllowRemoteConnections ){
						
								//ntohl( &(from.sin_addr) );
								if( bLogDebugToConsole ){
									printf( "*** Connection from: %s\n", remoteAddress );
								}
						
								processConnection(c);
								
							}else{
							
								sprintf( caDbg, "x-httpd: Access Denied: allow_remote: %i\n", bAllowRemoteConnections );
								//XPLMDebugString(caDbg);
								printf( caDbg );
							
								//htmlAccessDenied
								//TODO: send basic hard coded http packet with denied message
							
								close(c);
							}
						break;
				}
			}while( c != -1 );





}








void x_httpd::initSockets(){

	//	struct hostent *h;
	//	if ((h=gethostbyname("localhost")) == NULL) {  /* get the host info */
      ///      herror("gethostbyname");
      //      exit(1);
      //  }


		struct sockaddr_in sin;
		
		sin.sin_family=AF_INET;
		sin.sin_port=htons(1312);
			//net_aton("127.0.0.1", &(sin.sin_addr));
		sin.sin_addr.s_addr= htonl(INADDR_ANY);

		this->sock = socket( AF_INET, SOCK_STREAM, 0 );
		
		int sflag = 1;
		setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, (char*)&sflag, sizeof(int) );
		
		int sflags = fcntl( sock, F_GETFL );
		//fcntl( sock, F_SETFL, O_NONBLOCK );
		fcntl( sock, F_SETFL, sflags | O_NONBLOCK );
		
		if( bind( sock, (const sockaddr*)&sin, sizeof(sin) ) != 0 ){
			printf("x-httpd: Failed to bind server socket to port: %i\n", sin.sin_port);
			return;
		}else{
			listen( sock, 5 );
			printf("Socket listening on port %i.\n", ntohs(sin.sin_port));
		}
	

}








void x_httpd::header401Deny( char *header ){
	sprintf( header, "%s", 	
						"HTTP/1.0 401 Access Denied\n" 
						"Server: " X_HTTPD_VERSION_STRING "\n"
						"WWW-Authenticate: Basic realm=\"X-Plane\"\n"
						"Content-type: text/html\n"
						//"\n"
			);

}




void x_httpd::header200OK_MIME( char *header, const char* mime_string ){
	sprintf( header, 
						"HTTP/1.0 200 OK\n"	
						"Server: " X_HTTPD_VERSION_STRING "\n"
						"Content-type: %s\n",
						
						mime_string
			);
}



void x_httpd::header404NF( char *header ){
	sprintf( header, "%s", 
						"HTTP/1.0 404 Not Found\n"
						"Server: " X_HTTPD_VERSION_STRING "\n"
						"Content-Type: text/html\n"
						//"\n"
			);

}








int x_httpd::html404Document( char *header, char *html, char *requestString, char *queryString ){
	
	this->header404NF( header );

	int appVer, xplmVer;
	//XPLMHostApplicationID appId;
	//XPLMGetVersions( &appVer, &xplmVer, &appId );
	
	appVer=999;
	xplmVer=999;


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

int x_httpd::htmlAccessDenied( char *header, char *html ){

	this->header401Deny( header );
	sprintf( html, "%s", "Access Denied" );
	
	return strlen( html );

}



int x_httpd::htmlGeneric( char *header, char *html, char *payload ){

	this->header200OK_MIME( header, "text/html" );

	sprintf(html, "%s", 
						payload
						);
						
	return strlen( html );

}


int x_httpd::htmlSendBinary( char *header, char *html, unsigned char *buffer, int size, char *fileType ){


	//TODO: upgrade to use iterator.
	std::string sMimeTypeKey = std::string( fileType );
	
	typedef std::map<std::string, std::string> MapType;
	MapType::iterator iter;
		
	iter = xhttpd_mapMimeTypes.find( sMimeTypeKey );
	
	if( iter != xhttpd_mapMimeTypes.end() ){
		this->header200OK_MIME( header, iter->second.c_str() );
	}else{
		this->header200OK_MIME( header, "application/octet-stream" );
	}
	
		memset( html, 0, size );
		memcpy( html, buffer, size );

	return size;

}



//Universal dataref set.
int x_httpd::htmlUniSet( char *header, char *html ){
	this->header200OK_MIME( header, "text/html" );
	
	char drefName[1024];
	char drefType[1024];
	
	memset( drefName, 0, 1024 );
	memset( drefType, 0, 1024 );
	
	
	this->parseQuerystringForString( "dref", drefName, 1023 );
	this->parseQuerystringForString( "type", drefType, 1023 );
	//val is retrieved below depending on the dref type argument.
	
	/*
	XPLMDataRef tmpDr = XPLMFindDataRef( drefName );
	
	switch( drefType[0] ){
		case 'i':
			{
				int tmp = parseQuerystringForInt("val");
				XPLMSetDatai( tmpDr, tmp );
				//sprintf( drefValueFormatString, "Int Value: (%i)<br/>\n", tmp );
			}
			break;
		case 'f':
			{
				float tmp = parseQuerystringForFloat("val");
				XPLMSetDataf( tmpDr, tmp );
				//sprintf( drefValueFormatString, "Float Value: (%f)<br/>\n", tmp );
			}
			break;
		case 'c':
			{
				//sprintf( drefValueFormatString, "String Value: (%s)<br/>\n", "foo" );
			}
			break;		
	}
	*/
	
	
	
	//sprintf( html, "{ result:true }" );
		sprintf( html,
					"{ \"result\":true, \"type\":\"%s\", \"value\":%s, \"dref\":\"%s\" }",
						drefType, //type data for easier programming for the client.
						"_", //dynamically formatted above.
						drefName //echo the name for easier programming.
				);

	

	return strlen( html );

}


//Universal dataref get..
int x_httpd::htmlUniGet( char *header, char *html ){

	char caDbg[1024];

	this->header200OK_MIME( header, "text/html" );
	
	char drefName[1024];
	char drefType[1024];
	char drefStringVal[1024];
	
	memset( drefName, 0, 1024 );
	memset( drefType, 0, 1024 );
	memset( drefStringVal, 0, 1024 );
	
	
	
	this->parseQuerystringForString( "dref", drefName, 1023 );
	this->parseQuerystringForString( "type", drefType, 1023 );
	//val is retrieved below depending on the dref type argument.
	
	
	/*
	XPLMDataRef tmpDr = XPLMFindDataRef( drefName );
	
	
	//Check to see if we could find the dataref.
	if( tmpDr == NULL ){
		sprintf( html,
						"{ \"result\":false, \"type\":\"%s\", \"value\":%s, \"dref\":\"%s\" }",
							drefType, //type data for easier programming for the client.
							"404: Dataref not found.", //dynamically formatted above.
							drefName //echo the name for easier programming.
					);
						

		return strlen( html );
	}
	
	
	sprintf(caDbg, "  extracted dref type: %s\n", drefType);
	XPLMDebugString(caDbg);
	
	switch( drefType[0] ){
		case 'i':
			{
				//int tmp = parseQuerystringForInt("val");
				//XPLMSetDatai( tmpDr, tmp );
				int tmp = XPLMGetDatai( tmpDr );
				sprintf( drefStringVal, "%i", tmp );
			}
			break;
		case 'f':
			{
				//float tmp = parseQuerystringForFloat("val");
				//XPLMSetDataf( tmpDr, tmp );
				float tmp = XPLMGetDataf( tmpDr );
				sprintf( drefStringVal, "%0.5f", tmp );
			}
			break;
		case 'd':
			{
				//float tmp = parseQuerystringForFloat("val");
				//XPLMSetDataf( tmpDr, tmp );
				double tmp = XPLMGetDataf( tmpDr );
				sprintf( drefStringVal, "%0.5f", tmp );
			}
			break;
		case 'c':
			{
				sprintf( drefStringVal, "\"%s\"", "fixme_get_string_vals" );
			}
			break;		
	}
	*/
	
	
	sprintf( html,
					"{ \"result\":true, \"type\":\"%s\", \"value\":%s, \"dref\":\"%s\" }",
						drefType, //type data for easier programming for the client.
						drefStringVal, //dynamically formatted above.
						drefName //echo the name for easier programming.
				);
					

	return strlen( html );

}


#if 0
int x_httpd::htmlMiscStateXML( char *header, char *html, char *queryString ){

	//prepare GPS nav data.
	/*
	XPLMNavRef navRef = XPLMGetGPSDestination();
	XPLMNavType navType;// = XPLMGetGPSDestinationType();
	*/
	
	float gpsLat, gpsLon, gpsAlt;
	int gpsFreq;
	float gpsHeading;
	char gpsID[32];
	char gpsName[256];
	char gpsReg[256];

	/*
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

	*/



	//this function fires as the root(/) url, it relies on /index.xsl for formatting.

		this->header200OK_MIME( header, "text/plain" ); //FIXME: XML!

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

int x_httpd::htmlStateXML( char *header, char *html ){

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





		header200OK_MIME( header, "text/plain" ); //FIXME: XML!

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
#endif






void x_httpd::parseQuerystring( char *queryString ){

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

	
}


int x_httpd::parseQuerystringForInt( char *key ){
	char tmpret[32];
	parseQuerystringForString( key, tmpret, 32 );
	
	return atoi( tmpret );
}
float x_httpd::parseQuerystringForFloat( char *key ){
	char tmpret[32];
	parseQuerystringForString( key, tmpret, 32 );
	
	return atof( tmpret );
}


//This function is nasty.
void x_httpd::parseQuerystringForString( char *key, char *ret, int retSize ){

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






void x_httpd::processConnection( int c ){

	FILE *sockIn, *sockOut;

	char caDbg[1024];

		sockIn = fdopen( c, "r" );
			
			
			
			// ------ This read loop code needs replacing. ;-)
			
				//**this has been replaced with a global char* that is malloced.
				//char inbuf[INBUF_SIZE];
				
				//sprintf( caDbg, "x-httpd: processConnection(c)..\n" ); XPLMDebugString(caDbg);
				
				#define INBUF_SIZE 1024*1024*10
				
				memset(inbuf, 0, INBUF_SIZE);
				size_t bytes_read = 0;
				size_t chunk_size = 0;
				
				do{
					chunk_size = fread( inbuf + bytes_read, 1, 1, sockIn );
					bytes_read += chunk_size;
					//sprintf( caDbg, "  chunk read: %li bytes\n", chunk_size ); XPLMDebugString(caDbg);
					//usleep( 100 ); //do we need this?
				}while( chunk_size > 0 );
				
				
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
				
				
			// --- please?
			
			
			
		
			if( bLogDebugToConsole ){
				sprintf(caDbg, "---recv x-httpd request, %i bytes---\n", (int)strlen(inbuf));
				//XPLMDebugString(caDbg);
				
					sprintf(caDbg, "%s", inbuf );
					//XPLMDebugString(caDbg);
					
				sprintf(caDbg, "--- end recv ---\n");
				//XPLMDebugString(caDbg);
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



		//Parse the request header for it's METHOD, Query String and protocol version.
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
				//sprintf(caDbg, "x-httpd request: (%s)\n", requestString ); XPLMDebugString(caDbg);
			}
			
		}else{
			if( bLogDebugToConsole ){
				//sprintf(caDbg, "IRStart/IRStop; %i / %i\n", iRequestStringStart, iRequestStringStop ); XPLMDebugString(caDbg);
			}
		}


			//printf( "Parsed request(%i): QueryString: %i > %i\n", iStringLen, iRequestStringStart, iRequestStringStop );
			if( iQueryStringStart > -1 && iQueryStringStop > -1 ){
				memcpy( queryString, inbuf+iQueryStringStart, iQueryStringStop-iQueryStringStart );
				if( bLogDebugToConsole ){
					//sprintf(caDbg, "x-httpd request querystring: (%s)\n", queryString ); XPLMDebugString(caDbg);
				}
				
				
			}else{
				if( bLogDebugToConsole ){
					//sprintf( caDbg, "IQStart/IQStop; %i / %i\n", iQueryStringStart, iQueryStringStop ); XPLMDebugString(caDbg);
				}
				//the user has submitted an ivalid request and we could not parse the requested-file string out of it.
				//close( c );
				//return;
			}





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
							
							

							
			strcpy( queryString, fixUrlEntities.c_str() );
			if( bLogDebugToConsole ){
				//sprintf( caDbg, "Decoded URL: (%s)\n", queryString ); XPLMDebugString(caDbg);
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
		//memset(html, 0, BUFFER_PAGE_SIZE);
		
		char header[2048];
		memset(header, 0, 2048);


#pragma mark Request Processor here.

		
		int iPayloadSize = 0;

		if( 
			( authorizationToken == this->sAuthTokenB64 ) 
			|| ( ! bRequirePassword ) 
		){
		
			if( bLogDebugToConsole ){
				//sprintf( caDbg, "Authorization is good!\n"); XPLMDebugString(caDbg);
			}
		
				if( strcmp( requestString, "/about" ) == 0 ){
					//iPayloadSize = htmlGeneric( header, html, (char *)page_index );
					iPayloadSize = htmlGeneric( header, html, "x-httpd 14.12.06.1934 alpha" );

				//these items are dynamic
				/*
				}else if( strcmp( requestString, "/state.xml" ) == 0 ){
					iPayloadSize = htmlStateXML( header, html );

				}else if( strcmp( requestString, "/full-state.xml" ) == 0 ){
					iPayloadSize = htmlMiscStateXML( header, html, queryString ); //htmlRootDocument( header, html );
				*/
				
				}else if( strcmp( requestString, "/get" ) == 0 ){
					iPayloadSize = htmlUniGet( header, html );

				}else if( strcmp( requestString, "/set" ) == 0 ){
					iPayloadSize = htmlUniSet( header, html );


				}else{
					//we should look on the disk for this file.
					
					//EXPERIMENTAL
					//check to see if another plugin has registered to handle this resource..
					std::map<std::string, std::string>::iterator it = xhttpd_mapResourceMap.find( std::string(requestString) );
					
					if( it != xhttpd_mapResourceMap.end() ){
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
									sockOut = fdopen( c, "w" );

										fwrite( hack_blob, strlen(hack_blob), 1, sockOut );

										fflush( sockOut );

									fclose( sockIn );
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
								if( strcmp( requestString, "/" ) == 0 ){
									sprintf( requestString, "/index.htm" );
								}
								
								
								
								char webRoot[1024];
								findWebRoot( webRoot );
								//sprintf( caDbg, "webroot: %s\n", webRoot ); XPLMDebugString(caDbg);
								
								int iTempPayloadSize=0;
								
								char tmpFilename[2048];
								sprintf( tmpFilename, "html%s", requestString );
								//sprintf( caDbg, "Attempting to open and temp-cache the file: %s\n", tmpFilename ); XPLMDebugString(caDbg);
								
								
								//fixme; make the file-extension detector work with more than just three letter extensions.
								char tmpFileType[5];
								memset(	tmpFileType, 0, 5 );
								memcpy(
										tmpFileType,
										tmpFilename + (strlen(tmpFilename)-4), 
										4
										); //this should give us the file extension.
								
								//sprintf( caDbg, "File extension: %s\n", tmpFileType ); XPLMDebugString(caDbg);
								
								
								
								//fixme; check requested filename for ".." parent dir strings
								
								//todo; detect file type...
								cacheFile_Bin(webRoot, tmpFilename, &generic_cache, &iTempPayloadSize);
								
								if( iTempPayloadSize > 0 ){
									//sprintf( caDbg, "payload size returned by cacheFile_Bin: %i\n", iTempPayloadSize ); XPLMDebugString(caDbg);
								
									iPayloadSize = htmlSendBinary(header, html, generic_cache, iTempPayloadSize, tmpFileType);
									//iPayloadSize = htmlSendImagePNG( header, html, img_shadow, &img_shadow_size );
								
									free( generic_cache );
								}else{
									iPayloadSize = html404Document( header, html, requestString, queryString );
								}
								
					} //end check for registered filter for uri

				} //end of checks for builts-ins, registered-filters and static-files
				
				
				
				//double check content to see if we got a 404
				if( iPayloadSize == 0 ){
					iPayloadSize = html404Document( header, html, requestString, queryString );
				}
				

		}else{
			if( bLogDebugToConsole ){
				//sprintf( caDbg, "HTTP Auth is BAD!\n"); XPLMDebugString( caDbg );
			}
			if( bLogDebugToConsole ){ 
				//sprintf( caDbg, "recv: (%s) wanted: (%s)\n", authorizationToken.c_str(), (const char *)auth_token_b64); XPLMDebugString(caDbg);
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





//eof
