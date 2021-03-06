/*

This software is made available under the GNU Affero GPL Version 3 license. 
Please see agpl.txt for more information.

Copyright 2005,2013 - Ben Russell, br@x-plugins.com

*/


#if APL
	#include <Carbon/Carbon.h>
#endif


#pragma mark Include Headers


#include "global_defs.h"

#include "common_sdk.h"
#include "common_includes.h"


#include "io_utils.h"


#include "x_httpd.h"

x_httpd *httpd;


#include "drefs.h"


//quick hack globals to get server working.
int bAllowRemoteConnections = 1;
int bRequirePassword = 0;
int bLogDebugToConsole = 1;




XPLMMenuID gMenu = NULL;
void HandleMenuClick(
                                   void *               inMenuRef,    
                                   void *               inItemRef);


/*
enum{
	xhttpd_menuid_allow_remote,
	xhttpd_menuid_require_password = "require_password",
	xhttpd_menuid_log_debug_to_console = "console_log",
	xhttpd_menuid_change_password,
	xhttpd_menuid_refresh_cache,
	xhttpd_menuid_about
};
*/

int WidgetCallback(
                   XPWidgetMessage      inMessage,    
                   XPWidgetID           inWidget,    
                   long                 inParam1,    
                   long                 inParam2
                   );
                  


    int TOP_SHIFT=20;
    int BOT_SHIFT=12;
    int CONFIG_STRING_SIZE=256;



void dialog_ChangePassword();
XPWidgetID gRootChangePassword               = NULL;
XPWidgetID gChangePasswordWidgetTextName     = NULL;
XPWidgetID gChangePasswordWidgetTextPassword = NULL;
XPWidgetID gChangePasswordWidgetButtonOk     = NULL;
XPWidgetID gChangePasswordWidgetButtonCancel = NULL;

void dialog_About();
XPWidgetID gRootAbout = NULL;

        



#pragma mark Mandatory X-Plugin Signatures

PLUGIN_API int XPluginStart(
						char *		outName,
						char *		outSig,
						char *		outDesc);

PLUGIN_API void XPluginStop(void);

PLUGIN_API int XPluginEnable(void);

PLUGIN_API void XPluginDisable(void);

PLUGIN_API void XPluginReceiveMessage(
    XPLMPluginID   inFromWho,
    long        inMessage,
    void *      inParam);
	
	
#pragma mark -



#pragma mark Other
//this is an example flight loop function, it is not mandatory
float MyFlightLoopCallback(
                                   float                inElapsedSinceLastCall,    
                                   float                inElapsedTimeSinceLastFlightLoop,    
                                   int                  inCounter,    
                                   void *               inRefcon);






#pragma mark -
