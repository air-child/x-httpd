/*

This software is made available under the GNU Affero GPL Version 3 license. 
Please see agpl.txt for more information.

Copyright 2005,2013 - Ben Russell, br@x-plugins.com

*/


#include "main.h"


#pragma mark Mandatory X-Plugin Signatures

PLUGIN_API int XPluginStart(
						char *		outName,
						char *		outSig,
						char *		outDesc)
{
    strcpy(outName, "x-httpd");
    strcpy(outSig, "http://github.com/benrussell/x-httpd");
    strcpy(outDesc, "http server for X-Plane.");



        gMenu = XPLMCreateMenu(
                                "x-httpd",
                                XPLMFindPluginsMenu(),
                                    XPLMAppendMenuItem( 
                                                        XPLMFindPluginsMenu(),
                                                        "x-httpd",
                                                        0,
                                                        1
                                                      ),
                                HandleMenuClick,
                                0
                                );
								
								
						XPLMAppendMenuItem(
											gMenu,
											"Allow remote connections.",
											(void *)"allow_remote", //item ref
											1 //force english
											);
								XPLMCheckMenuItem( gMenu, 0, xplm_Menu_Unchecked );

						XPLMAppendMenuItem(
											gMenu,
											"Require password.",
											(void *)"require_password", //item ref
											1 //force english
											);
								XPLMCheckMenuItem( gMenu, 1, xplm_Menu_Checked );


						XPLMAppendMenuItem(
											gMenu,
											"Log to console.",
											(void *)"console_log", //item ref
											1 //force english
											);
								XPLMCheckMenuItem( gMenu, 2, xplm_Menu_Unchecked );



						XPLMAppendMenuItem(
											gMenu,
											"Change password...",
											(void *)"change_password", //item ref
											1 //force english
											);

						XPLMAppendMenuItem(
											gMenu,
											"Refresh file cache.",
											(void *)"refresh_cache", //item ref
											1 //force english
											);

						XPLMAppendMenuItem(
											gMenu,
											"About...",
											(void *)"about", //item ref
											1 //force english
											);






	prepareFileCache();

		xpdr_Lat = XPLMFindDataRef("sim/flightmodel/position/latitude");
		xpdr_Lon = XPLMFindDataRef("sim/flightmodel/position/longitude");
		xpdr_AltMSL = XPLMFindDataRef("sim/flightmodel/position/elevation");
		xpdr_AltAGL = XPLMFindDataRef("sim/flightmodel/position/y_agl");

		xpdr_X = XPLMFindDataRef("sim/flightmodel/position/local_x");
		xpdr_Y = XPLMFindDataRef("sim/flightmodel/position/local_y");
		xpdr_Z = XPLMFindDataRef("sim/flightmodel/position/local_z");

		xpdr_Heading = XPLMFindDataRef("sim/flightmodel/position/psi");
		xpdr_HeadingMag = XPLMFindDataRef("sim/flightmodel/position/magpsi");
		xpdr_HeadingTrack = XPLMFindDataRef("sim/flightmodel/position/hpath");
		
		xpdr_Kias = XPLMFindDataRef("sim/flightmodel/position/indicated_airspeed"); //knots
		xpdr_Ktas = XPLMFindDataRef("sim/flightmodel/position/true_airspeed"); //mtrs/sec
		xpdr_Kgs = XPLMFindDataRef("sim/flightmodel/position/groundspeed"); //mtrs/sec
		
		xpdr_WindDirection = XPLMFindDataRef("sim/weather/wind_direction_degt");  //direction at acf
		xpdr_WindSpeed = XPLMFindDataRef("sim/weather/wind_speed_kt");	//speed at acf
		
		xpdr_Nav1 = XPLMFindDataRef("sim/cockpit/radios/nav1_freq_hz");
		xpdr_Nav1s = XPLMFindDataRef("sim/cockpit/radios/nav1_stdby_freq_hz");

		xpdr_Com1 = XPLMFindDataRef("sim/cockpit/radios/com1_freq_hz");
		xpdr_Com1s = XPLMFindDataRef("sim/cockpit/radios/com1_stdby_freq_hz");


		


		//misc-state.xml data refs.
		 xpdr_windAltHigh = XPLMFindDataRef("sim/weather/wind_altitude_msl_m[2]");
		 xpdr_windSpeedHigh = XPLMFindDataRef("sim/weather/wind_speed_kt[2]");
		 xpdr_windDirHigh = XPLMFindDataRef("sim/weather/wind_direction_degt[2]");
		 xpdr_windSheerSpeedHigh = XPLMFindDataRef("sim/weather/shear_speed_kt[2]");
		 xpdr_windSheerDirectionHigh = XPLMFindDataRef("sim/weather/shear_direction_degt[2]");
		 xpdr_windTurbulenceHigh = XPLMFindDataRef("sim/weather/turbulence[2]");

		 xpdr_windAltMed = XPLMFindDataRef("sim/weather/wind_altitude_msl_m[1]");
		 xpdr_windSpeedMed = XPLMFindDataRef("sim/weather/wind_speed_kt[1]");
		 xpdr_windDirMed = XPLMFindDataRef("sim/weather/wind_direction_degt[1]");
		 xpdr_windSheerSpeedMed = XPLMFindDataRef("sim/weather/shear_speed_kt[1]");
		 xpdr_windSheerDirectionMed = XPLMFindDataRef("sim/weather/shear_direction_degt[1]");
		 xpdr_windTurbulenceMed = XPLMFindDataRef("sim/weather/turbulence[1]");

		 xpdr_windAltLow = XPLMFindDataRef("sim/weather/wind_altitude_msl_m[0]");
		 xpdr_windSpeedLow = XPLMFindDataRef("sim/weather/wind_speed_kt[0]");
		 xpdr_windDirLow = XPLMFindDataRef("sim/weather/wind_direction_degt[0]");
		 xpdr_windSheerSpeedLow = XPLMFindDataRef("sim/weather/shear_speed_kt[0]");
		 xpdr_windSheerDirectionLow = XPLMFindDataRef("sim/weather/shear_direction_degt[0]");
		 xpdr_windTurbulenceLow = XPLMFindDataRef("sim/weather/turbulence[0]");

			//need to add the rest of the weather screen!



    return 1;
}



void HandleMenuClick(void *inMenuRef, void *inItemRef){


	if( strcmp( (char*)inItemRef, "about") == 0 ){
		if( gRootAbout == NULL ){
			dialog_About();
		}
		
	}else if( strcmp( (char*)inItemRef, "allow_remote") == 0 ){
                    if( bAllowRemoteConnections ){
						bAllowRemoteConnections = 0;
							XPLMCheckMenuItem( gMenu, 0, xplm_Menu_Unchecked );
							XPLMSpeakString("x-httpd; Remote connections denied.");
					}else{
						bAllowRemoteConnections = 1;
							XPLMCheckMenuItem( gMenu, 0, xplm_Menu_Checked );
							XPLMSpeakString("x-httpd; Remote connections allowed. Anyone on your network can now connect to X-Plane. This may include the internet.");				
					}	
				
	}else if( strcmp( (char*)inItemRef, "require_password") == 0 ){
					if( bRequirePassword ){
						bRequirePassword = 0;
							XPLMCheckMenuItem( gMenu, 1, xplm_Menu_Unchecked );
							XPLMSpeakString("x-httpd; Password protection is now OFF.");
					}else{
						bRequirePassword = 1;
							XPLMCheckMenuItem( gMenu, 1, xplm_Menu_Checked );
							XPLMSpeakString("x-httpd; Password protection on.");				
					}

	}else if( strcmp( (char*)inItemRef, "change_password") == 0 ){
					if( gRootChangePassword == NULL ){
						dialog_ChangePassword();					
					}

	}else if( strcmp( (char*)inItemRef, "refresh_cache") == 0 ){
					cleanupFileCache();
					prepareFileCache();
					XPLMSpeakString("x-httpd; Content has been refreshed.");

	}else if( strcmp( (char*)inItemRef, "console_log") == 0 ){
					if( bLogDebugToConsole ){
						bLogDebugToConsole = 0;
						XPLMCheckMenuItem( gMenu, 2, xplm_Menu_Unchecked );
					}else{
						bLogDebugToConsole = 1;
						XPLMCheckMenuItem( gMenu, 2, xplm_Menu_Checked );
					}
		
	} //end decide which menu was clicked.


}



int changePassword(){

	int r=0;

		int newDetailsError = 0;
	
		char newUsername[256];
		char newPassword[256];
			XPGetWidgetDescriptor(gChangePasswordWidgetTextName, newUsername, 256);
			XPGetWidgetDescriptor(gChangePasswordWidgetTextPassword, newPassword, 256);
			
			
			for( int x=0; x < (int)strlen( newUsername ); x++ ){
				if( newUsername[x] == ':' ){
					XPLMSpeakString("Found : in username.");
					newDetailsError = 1;
					break;
				}
			}
			for( int x=0; x < (int)strlen( newPassword ); x++ ){
				if( newPassword[x] == ':' ){
					XPLMSpeakString("Found : in password.");
					newDetailsError = 1;
					break;
				}
			}
			
		if( newDetailsError == 0 ){
			//new auth details are good.
			
			char plain[1024] = "";
			memset( plain, 0, 1024 );
			sprintf( plain, "%s:%s", newUsername, newPassword );
			
			char ret[1024] = "";
			memset( ret, 0, 1024 );
			encode_string( plain, ret );
			
			printf( "new auth tokens: %s\n", plain );
			printf( "encoded tokens: %s\n", ret );
			
		
			free( auth_token_raw );
			free( auth_token_b64 );
			auth_token_raw = (unsigned char*)malloc( strlen(plain) );
			auth_token_b64 = (unsigned char*)malloc( strlen(ret) );
			
			strcpy( (char *)auth_token_raw, (const char*)plain );
			strcpy( (char *)auth_token_b64, (const char*)ret );
			
			
			
			char webRoot[1024] = "";
			findWebRoot( webRoot );
			
			char filename[2048] = "";
			sprintf(filename, "%s/password.txt", webRoot);
			
			FILE *fout = fopen(filename, "w");
				if( fout ){
					fwrite( plain, 1, strlen(plain), fout);
					fclose( fout );
				}
			
			XPLMSpeakString("x-httpd; password changed.");
			XPDestroyWidget( gRootChangePassword, 1 );
			gRootChangePassword = NULL;
			
			r = 1;
		
		}


	return r;


}





int WidgetCallback(
                   XPWidgetMessage      inMessage,    
                   XPWidgetID           inWidget,    
                   long                 inParam1,    
                   long                 inParam2
                   ){
                  
        //tab wrapper - doesn't work
		/*
			if( inMessage == xpMsg_KeyPress ){
				printf("key pressed..\n");
				if( inParam1 == -1073744608 ){ //using a defined symbol doesn't apply here. -_-
					printf("\tkey is TAB, losing focus.\n");
					XPLoseKeyboardFocus( inWidget );
					return 1;
				}else{
					printf("\tkey is unkown p1/p2; (%li/%li)\n", inParam1, inParam2);
				}
			}
		*/
		
		
            int r = 0; // the return value, assume failure.
            
            
            if(inWidget == gRootChangePassword){
                if( inMessage == xpMessage_CloseButtonPushed ){
                    XPDestroyWidget( gRootChangePassword, 1 );
                    gRootChangePassword = NULL;
                    r=1;
                }
                
            }else if(inWidget == gChangePasswordWidgetButtonOk){
				 if( inMessage == xpMsg_PushButtonPressed ){
					//XPLMSpeakString("pushed OK button.");
					r = changePassword();
				 }
            }else if(inWidget == gChangePasswordWidgetButtonCancel){
				 if( inMessage == xpMsg_PushButtonPressed ){
                    XPDestroyWidget( gRootChangePassword, 1 );
                    gRootChangePassword = NULL;
                    r=1;					
				 }
				 
			//--- now deal with messages from about dialog.
				 
			}else if(inWidget == gRootAbout){
                if( inMessage == xpMessage_CloseButtonPushed ){
                    XPDestroyWidget( gRootAbout, 1 );
                    gRootAbout = NULL;
                    r=1;
                }
			}
			

	return r;
}


void dialog_ChangePassword(){
	int     left, top, right, bottom, height, width,//static, known sizes
                tleft, ttop, tright, tbottom; //dynamic to ease widget placement somewhat

    height = 130;
    width = 250;


        left = (1024/2)-(width/2);
        tleft = left;
        top = (768/2)+(height/2);
        ttop = top;

            right = left+width;
            tright = right;
            bottom = top-height;
            tbottom = bottom;



	gRootChangePassword = XPCreateWidget(left, top, right, bottom,
                        1, "x-httpd - change password", 1, 
                        0, 
                        xpWidgetClass_MainWindow
                        );


    XPSetWidgetProperty(gRootChangePassword, xpProperty_MainWindowHasCloseBoxes, 1);
    XPAddWidgetCallback( gRootChangePassword, WidgetCallback );


    tleft+=20; //move to the center a bit for the section headings
    tright = tleft+200;

    ttop-=35; //move down from the top a bit to clear the window caption etc.
    tbottom = ttop - 12;


    int iFieldOffset = 70;


		XPCreateWidget(tleft+15, ttop+2, tright, tbottom,
                    1, "Change web-server password...", 0, 
                    gRootChangePassword, 
                    xpWidgetClass_Caption
                    );


		ttop-=TOP_SHIFT-5; //move down from the top a bit to clear the window caption etc.
		tbottom = ttop - BOT_SHIFT-5;

        XPCreateWidget(tleft+10, ttop, tright, tbottom,
                        1, "username:", 0, 
                        gRootChangePassword, 
                        xpWidgetClass_Caption
                        );
      
        gChangePasswordWidgetTextName = XPCreateWidget(tleft+iFieldOffset, ttop, tright, tbottom,
                                            1, "", 0, 
                                            gRootChangePassword, 
                                            xpWidgetClass_TextField
                                            );

		ttop-=TOP_SHIFT;//-10; //move down from the top a bit to clear the window caption etc.
		tbottom = ttop - BOT_SHIFT;//-10;
		

        XPCreateWidget(tleft+10, ttop, tright, tbottom,
                    1, "password:", 0, 
                    gRootChangePassword, 
                    xpWidgetClass_Caption
                    );

        gChangePasswordWidgetTextPassword = XPCreateWidget(tleft+iFieldOffset, ttop, tright, tbottom,
                                        1, "", 0, 
                                        gRootChangePassword, 
                                        xpWidgetClass_TextField
                                        );

		ttop-=TOP_SHIFT; //move down from the top a bit to clear the window caption etc.
		tbottom = ttop - BOT_SHIFT;



		gChangePasswordWidgetButtonCancel = XPCreateWidget(left+10, bottom+30, tleft+90, bottom,
										1, "Cancel", 0, 
										gRootChangePassword, 
										xpWidgetClass_Button
										);
				XPAddWidgetCallback( gChangePasswordWidgetButtonCancel, WidgetCallback );


		gChangePasswordWidgetButtonOk = XPCreateWidget(right-100, bottom+30, right-10, bottom,
										1, "OK", 0, 
										gRootChangePassword, 
										xpWidgetClass_Button
										);
				XPAddWidgetCallback( gChangePasswordWidgetButtonOk, WidgetCallback );


}//end dialog_ChangePassword




void dialog_About(){
	int     left, top, right, bottom, height, width,//static, known sizes
                tleft, ttop, tright, tbottom; //dynamic to ease widget placement somewhat

    height = 100;
    width = 290;



        left = (1024/2)-(width/2);
        tleft = left;
        top = (768/2)+(height/2);
        ttop = top;

            right = left+width;
            tright = right;
            bottom = top-height;
            tbottom = bottom;



	gRootAbout = XPCreateWidget(left, top, right, bottom,
                        1, "x-httpd - about", 1, 
                        0, 
                        xpWidgetClass_MainWindow
                        );


    XPSetWidgetProperty(gRootAbout, xpProperty_MainWindowHasCloseBoxes, 1);
    XPAddWidgetCallback( gRootAbout, WidgetCallback );


    tleft+=20; //move to the center a bit for the section headings
    tright = tleft+200;

    ttop-=35; //move down from the top a bit to clear the window caption etc.
    tbottom = ttop - 12;


    int iFieldOffset = 70;


		XPCreateWidget(tleft+15, ttop+2, tright, tbottom,
                    1, X_HTTPD_VERSION_STRING, 0, 
                    gRootAbout,
                    xpWidgetClass_Caption
                    );

		ttop-=TOP_SHIFT;//-10; //move down from the top a bit to clear the window caption etc.
		tbottom = ttop - BOT_SHIFT;//-10;

		XPCreateWidget(tleft+15+20, ttop+2, tright, tbottom,
                    1, "Compiled: " __DATE__ "  @ " __TIME__, 0, 
                    gRootAbout,
                    xpWidgetClass_Caption
                    );





		ttop-=TOP_SHIFT-5; //move down from the top a bit to clear the window caption etc.
		tbottom = ttop - BOT_SHIFT-5;

/*
        XPCreateWidget(tleft+10, ttop, tright, tbottom,
                        1, "username:", 0, 
                        gRootChangePassword, 
                        xpWidgetClass_Caption
                        );
      
        gChangePasswordWidgetTextName = XPCreateWidget(tleft+iFieldOffset, ttop, tright, tbottom,
                                            1, "", 0, 
                                            gRootChangePassword, 
                                            xpWidgetClass_TextField
                                            );

		ttop-=TOP_SHIFT;//-10; //move down from the top a bit to clear the window caption etc.
		tbottom = ttop - BOT_SHIFT;//-10;
		

        XPCreateWidget(tleft+10, ttop, tright, tbottom,
                    1, "password:", 0, 
                    gRootChangePassword, 
                    xpWidgetClass_Caption
                    );

        gChangePasswordWidgetTextPassword = XPCreateWidget(tleft+iFieldOffset, ttop, tright, tbottom,
                                        1, "", 0, 
                                        gRootChangePassword, 
                                        xpWidgetClass_TextField
                                        );

		ttop-=TOP_SHIFT; //move down from the top a bit to clear the window caption etc.
		tbottom = ttop - BOT_SHIFT;



		gChangePasswordWidgetButtonCancel = XPCreateWidget(left+10, bottom+30, tleft+90, bottom,
										1, "Cancel", 0, 
										gRootChangePassword, 
										xpWidgetClass_Button
										);
				XPAddWidgetCallback( gChangePasswordWidgetButtonCancel, WidgetCallback );

		gChangePasswordWidgetButtonOk = XPCreateWidget(right-100, bottom+30, right-10, bottom,
										1, "OK", 0, 
										gRootChangePassword, 
										xpWidgetClass_Button
										);
				XPAddWidgetCallback( gChangePasswordWidgetButtonOk, WidgetCallback );

*/

}//end dialog_ChangePassword





PLUGIN_API void XPluginStop(void){

	cleanupFileCache();

	close( sock ); //close the server socket.

}


PLUGIN_API int XPluginEnable(void){
        
        XPLMRegisterFlightLoopCallback(		
			MyFlightLoopCallback,	// Callback 
			-1.0,					// Interval 
			NULL);	

    return 1;
}

PLUGIN_API void XPluginDisable(void){

        // Unregister the callback
            XPLMUnregisterFlightLoopCallback(MyFlightLoopCallback, NULL);

}


PLUGIN_API void XPluginReceiveMessage(
    XPLMPluginID   inFromWho,
    long        inMessage,
    void *      inParam){
    
    
}
#pragma mark -

void initSockets(){

	//	struct hostent *h;
	//	if ((h=gethostbyname("localhost")) == NULL) {  /* get the host info */
      ///      herror("gethostbyname");
      //      exit(1);
      //  }


		struct sockaddr_in sin;
		
		sin.sin_family=AF_INET;
		sin.sin_port= htons(1312);
			//net_aton("127.0.0.1", &(sin.sin_addr));
		sin.sin_addr.s_addr= htonl(INADDR_ANY);

		sock = socket( AF_INET, SOCK_STREAM, 0 );
		
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
			printf("Socket listening on port %i.\n", sin.sin_port);
		}
	

}


#pragma mark Other
//this is an example flight loop function, it is not mandatory
PLUGIN_API float MyFlightLoopCallback(
                                   float                inElapsedSinceLastCall,    
                                   float                inElapsedTimeSinceLastFlightLoop,    
                                   int                  inCounter,    
                                   void *               inRefcon)
{


		char caDbg[1024];


			static int socketsInit = 0;
			if( ! socketsInit ){
			
				if( strcmp("username:password", (const char*)auth_token_raw) == 0 ){
					XPLMSpeakString("x-httpd; This is the first time you have used x-httpd, please change the password.");
					dialog_ChangePassword();
				
				}
				
				printf("Running sockets init...\n");
				initSockets();
				socketsInit = 1;
			
			}


			int c = 0;

			int clientCount = 0;
			do{
				clientCount++;
			
				if( clientCount >= 10 ){ break; } //we want to give x-plane priority, 10 clients per frame event.
			
				struct sockaddr_in from;
				memset( &from, 0, sizeof( sockaddr_in ));
				socklen_t len;

				//printf( "foo\n");
				
				c = accept( sock, (sockaddr*)&from, &len );				
				
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
						break;
					default:
						//printf("accept()ed a connect: %i\n", c);
						
						char remoteAddress[32];
							strcpy( remoteAddress, inet_ntoa(from.sin_addr) );
							
							sprintf( caDbg, "\nx-httpd: request from: %s\n", remoteAddress );
							XPLMDebugString(caDbg);
							
							
							//if(true){
							if( (strcmp( "127.0.0.1", remoteAddress ) == 0) || bAllowRemoteConnections ){
						
								//ntohl( &(from.sin_addr) );
								if( bLogDebugToConsole ){
									printf( "*** Connection from: %s\n", remoteAddress );
								}
						
								processConnection(c);
								
							}else{
							
								sprintf( caDbg, "x-httpd: Access Denied: allow_remote: %i\n", bAllowRemoteConnections );
								XPLMDebugString(caDbg);
							
								close(c);
							}
						break;
				}
			}while( c != -1 );


    //come get me next frame.
    return -1.0f;
    
}






void findPluginFolder(char *buffer){


    const char *sep = XPLMGetDirectorySeparator();

    char sysPath[512];
    char *pSysPath = sysPath;

    char tmp[1024];
    char *ptmp = tmp;

	char tmp2[1024];

    XPLMGetSystemPath(sysPath);

    sprintf(ptmp, "%sResources%cplugins%c", pSysPath, *sep, *sep);
        
#if APL

	sprintf(tmp2, "/Volumes/%s", tmp);
	strcpy( tmp, tmp2 );

//replace all occurences of the colon seperator with /'s
	
	int x=0;
    for(x=0; x<(int)strlen(tmp); x++){
        if( tmp[x] == ':' ){
            tmp[x] = '/';
        }    
    }
#endif    

//send it back via the pointer passed in.

	//printf("Plugins folder: %s\n", tmp);

    strcpy(buffer, ptmp);

}

void findWebRoot( char *buffer ){

	char pluginFolderPath[1024];
	findPluginFolder( pluginFolderPath );
	
	//printf("plugin folder: %s\n", pluginFolderPath );

	#if APL
		sprintf( buffer, "%sx-httpd.x-plugin/root/", pluginFolderPath );

	#endif


}


