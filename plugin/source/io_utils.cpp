//
//  io_utils.cpp
//  x-httpd
//
//  Created by Ben on 7/12/2014.
//
//

#include "io_utils.h"



size_t getFileSize( const char *filename, char* error_message ){

	struct stat fsize;
	
	
	if( !stat(filename, &fsize) ){
		return fsize.st_size;
	}else{
		sprintf( error_message, "No such file (stat error): %s\n", filename);		
	}
	
	return -1;

}




#ifdef XPLM200

void findPluginFolder(char *buffer){

    char caSysPath[1024];
    XPLMGetSystemPath(caSysPath);
	
	char caPluginFolderPath[2048];
    sprintf( caPluginFolderPath, "%sResources/plugins/", caSysPath );
        

    strcpy(buffer, caPluginFolderPath);

}


//XPL version
void findWebRoot( char *buffer ){

	char pluginFolderPath[2048];
	findPluginFolder( pluginFolderPath );
	
	sprintf( buffer, "%sx-httpd.x-plugin/x-httpd-content/", pluginFolderPath );

}


#else

//CLI debug test harness version, server path is hard coded.
void findWebRoot( char *buffer ){

	sprintf( buffer, "/Applications/X-Plane 10 beta/Resources/plugins/x-httpd.x-plugin/x-httpd-content/" );

}

#endif





//eof
