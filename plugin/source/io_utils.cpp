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







void findWebRoot( char *buffer ){

	char pluginFolderPath[1024];
	//findPluginFolder( pluginFolderPath );
	
	//printf("plugin folder: %s\n", pluginFolderPath );

	#if APL
	//	sprintf( buffer, "%sx-httpd.x-plugin/x-httpd-content/", pluginFolderPath );

	#endif


	sprintf( pluginFolderPath, "very broken." );

}










//eof
