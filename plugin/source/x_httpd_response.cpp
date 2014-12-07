//
//  x_httpd_response.cpp
//  x-httpd
//
//  Created by Ben on 7/12/2014.
//
//

#include "x_httpd_response.h"



x_httpd_response::x_httpd_response(){

	this->mapMimeTypes[".htm"] = "text/html";
	this->mapMimeTypes[".js"] = "text/plain"; //FIXME
	this->mapMimeTypes[".txt"] = "text/plain";
	this->mapMimeTypes[".css"] = "test/css";
	this->mapMimeTypes[".xml"] = "application/xml";
	this->mapMimeTypes[".png"] = "image/png";
	this->mapMimeTypes[".ico"] = "image/x-icon";
	this->mapMimeTypes[".swf"] = "application/x-shockwave-flash";

	this->mapMimeTypes[".bin"] = "application/octet-stream";


	//10 meg buffer.
	//	#define BUFSIZE 1024*1024*10
	//this->response = (char*)malloc( BUFSIZE );


}


x_httpd_response::~x_httpd_response(){

}



void x_httpd_response::header401Deny( char *header ){
	sprintf( header, "%s", 	
						"HTTP/1.0 401 Access Denied\n" 
						"Server: " X_HTTPD_VERSION_STRING "\n"
						"WWW-Authenticate: Basic realm=\"X-Plane\"\n"
						"Content-type: text/html\n"
						//"\n"
			);

}




void x_httpd_response::header200OK_MIME( char *header, const char* mime_string ){
	sprintf( header, 
						"HTTP/1.0 200 OK\n"	
						"Server: " X_HTTPD_VERSION_STRING "\n"
						"Content-type: %s\n",
						
						mime_string
			);
}



void x_httpd_response::header404NF( char *header ){
	sprintf( header, "%s", 
						"HTTP/1.0 404 Not Found\n"
						"Server: " X_HTTPD_VERSION_STRING "\n"
						"Content-Type: text/html\n"
						//"\n"
			);

}








int x_httpd_response::html404Document( char *header, char *html, const char *requestString, const char *queryString ){
	
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

int x_httpd_response::htmlAccessDenied( char *header, char *html ){

	this->header401Deny( header );
	sprintf( html, "%s", "Access Denied" );
	
	return strlen( html );

}



int x_httpd_response::htmlGeneric( char *header, char *html, char *payload ){

	this->header200OK_MIME( header, "text/html" );

	sprintf(html, "%s", 
						payload
						);
						
	return strlen( html );

}


int x_httpd_response::htmlSendBinary( char *header, char *html, unsigned char *buffer, int size, char *fileType ){


	//TODO: upgrade to use iterator.
	std::string sMimeTypeKey = std::string( fileType );
	
	typedef std::map<std::string, std::string> MapType;
	MapType::iterator iter;
		
	iter = mapMimeTypes.find( sMimeTypeKey );
	
	if( iter != mapMimeTypes.end() ){
		this->header200OK_MIME( header, iter->second.c_str() );
	}else{
		this->header200OK_MIME( header, "application/octet-stream" );
	}
	
		memset( html, 0, size );
		memcpy( html, buffer, size );

	return size;

}



//Universal dataref set.
int x_httpd_response::htmlUniSet( char *header, char *html ){
	this->header200OK_MIME( header, "text/html" );
	
	char drefName[1024];
	char drefType[1024];
	
	memset( drefName, 0, 1024 );
	memset( drefType, 0, 1024 );
	
	
	//this->parseQuerystringForString( "dref", drefName, 1023 );
	//this->parseQuerystringForString( "type", drefType, 1023 );
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
int x_httpd_response::htmlUniGet( char *header, char *html ){

	char caDbg[1024];

	this->header200OK_MIME( header, "text/html" );
	
	char drefName[1024];
	char drefType[1024];
	char drefStringVal[1024];
	
	memset( drefName, 0, 1024 );
	memset( drefType, 0, 1024 );
	memset( drefStringVal, 0, 1024 );
	
	
	
	//this->parseQuerystringForString( "dref", drefName, 1023 );
	//this->parseQuerystringForString( "type", drefType, 1023 );
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
int x_httpd_response::htmlMiscStateXML( char *header, char *html, char *queryString ){

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

int x_httpd_response::htmlStateXML( char *header, char *html ){

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




