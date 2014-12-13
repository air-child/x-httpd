/*

This software is made available under the GNU Affero GPL Version 3 license. 
Please see agpl.txt for more information.

Copyright 2005,2013 - Ben Russell, br@x-plugins.com

*/


#include "main_xpl.h"


#pragma mark Mandatory X-Plugin Signatures

PLUGIN_API int XPluginStart(
						char *		outName,
						char *		outSig,
						char *		outDesc)
{
    strcpy(outName, "x-httpd");
    strcpy(outSig, "http://github.com/benrussell/x-httpd");
    strcpy(outDesc, "http server for X-Plane.");
	
	XPLMEnableFeature("XPLM_USE_NATIVE_PATHS", 1);
	
	
	char caDbg[8192];

	char caPluginFolder[2048];
	char caWebRoot[2048];
	
	findPluginFolder( caPluginFolder );
	findWebRoot( caWebRoot );
	
	
	sprintf( caDbg, "x-httpd: plugin folder: (%s)\n", caPluginFolder);
	XPLMDebugString( caDbg );
	
	sprintf( caDbg, "x-httpd: web root: (%s)\n", caWebRoot);
	XPLMDebugString( caDbg );
	
	

		char caHostName[255];
		gethostname(caHostName, 255);
		
		struct hostent *host_entry;
		host_entry = gethostbyname( caHostName );
		
		char *caLocalIP;
		caLocalIP = inet_ntoa( *(struct in_addr *)*host_entry->h_addr_list);
		
		sprintf( caDbg, "x-httpd: hostname: %s ip: %s\n", caHostName, caLocalIP );
		XPLMDebugString( caDbg );



		
		//create httpd instance.
		httpd = new x_httpd( 1312 );
		
		//work is handled during FLCB cycles.
		


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

/*
	}else if( strcmp( (char*)inItemRef, "refresh_cache") == 0 ){
					cleanupFileCache();
					prepareFileCache();
					XPLMSpeakString("x-httpd; Content has been refreshed.");
*/
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
			
		
			/*
			free( auth_token_raw );
			free( auth_token_b64 );
			auth_token_raw = (unsigned char*)malloc( strlen(plain) );
			auth_token_b64 = (unsigned char*)malloc( strlen(ret) );
			
			strcpy( (char *)auth_token_raw, (const char*)plain );
			strcpy( (char *)auth_token_b64, (const char*)ret );
			*/
			
			
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
                    1, XHTTPD_SERVER_MESSAGE, 0, 
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

}//end dialog_ChangePassword





PLUGIN_API void XPluginStop(void){


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
    

	char caDbg[1024];


	char outName[1024];
	char outPath[1024];
	char outSig[1024];
	char outDescription[1024];

		XPLMGetPluginInfo( inFromWho, outName, outPath, outSig, outDescription );
	
		
		sprintf( caDbg, "x-httpd: ixplc msg from: %s\n", outSig ); XPLMDebugString( caDbg );
		
		long REG_URI = 0x0100c0de;
		long SEND_BLOB = 0x0100b10b;
		
		if( inMessage == SEND_BLOB ){
			//This code buffers data returning from other plugins.
			//We use the buffer elsewhere to complete http client requests.
		
			int packet_size;
			memcpy( &packet_size, inParam, 4 );
			
			sprintf( caDbg, "x-httpd: blob size: %i bytes\n", packet_size );
			
			//basic hack_blob protection.
			if( packet_size < 8129 ){
			
				memset( hack_blob,0,8192 ); //reset target buffer.
				memcpy( hack_blob, (char*)inParam+4, packet_size );
				
				XPLMDebugString("x-httpd: hack_blob filled.\n");
			
			}
		
				
		}else if( inMessage == REG_URI ){

			//xhttpd_map_Responders["/gizmo"] = "gizmo.x-plugins.com";
			
			sprintf( caDbg, "x-httpd: DISABLED: register uri: (%s) -> (%s)\n", (char*)inParam, outSig );
			XPLMDebugString( caDbg );
			
			
				
		}//end if-tree
		
	
	
	
	
    
}
#pragma mark -



#pragma mark Other
//this is an example flight loop function, it is not mandatory
float MyFlightLoopCallback(
                                   float                inElapsedSinceLastCall,    
                                   float                inElapsedTimeSinceLastFlightLoop,    
                                   int                  inCounter,    
                                   void *               inRefcon)
{


		httpd->run_slice( 5000 ); //max 5ms for web server work.


    //come get me next frame.
    return -1.0f;
    
}



//eof
