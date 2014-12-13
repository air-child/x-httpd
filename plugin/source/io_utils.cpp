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
    sprintf( caPluginFolderPath, "%s/Resources/plugins/", caSysPath );
        

	#if APL_HFS
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


    strcpy(buffer, caPluginFolderPath);

}



void findWebRoot( char *buffer ){

	char pluginFolderPath[2048];
	findPluginFolder( pluginFolderPath );
	
	#if APL
		sprintf( buffer, "%sx-httpd.x-plugin/x-httpd-content/", pluginFolderPath );

	#endif

}

#endif





//eof
