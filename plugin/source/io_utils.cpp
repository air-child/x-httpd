//
//  io_utils.cpp
//  x-httpd
//
//  Created by Ben on 7/12/2014.
//
//

#include "io_utils.h"







void cacheFile_Text( char *rootFolder, char *filename, unsigned char **buffer ){

	char targetFile[ strlen(rootFolder) + strlen(filename) + 2 ];

	sprintf( targetFile, "%s%s", rootFolder, filename );

	char caDbg[1024];
	//sprintf( caDbg, "attempting to cache text file: %s\n", targetFile ); XPLMDebugString( caDbg );


	int iSize = getFileSize( targetFile, caDbg ) + 1; //we add 1 to the end to ensure we have a terminator byte
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
	
	char caDbg[1024];

	sprintf( targetFile, "%s%s", rootFolder, filename );
	//sprintf( caDbg, "attempting to cache binary file: %s\n", targetFile ); XPLMDebugString( caDbg );

	*iSize = getFileSize( targetFile, caDbg );
	
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

	char caDbg[1024];

	char webRoot[1024];
	findWebRoot( webRoot );
	
	/*
	sprintf(caDbg, "webroot: %s\n", webRoot ); XPLMDebugString(caDbg);
	
		//cache authentication details
		cacheFile_Text( webRoot, "password.txt", &auth_token_raw );
				int b64_len = ((strlen((const char*)auth_token_raw))*2); //we multiply the buffer size by 2 to make sure we have room for the b64 version.
				auth_token_b64 = (unsigned char*)malloc( b64_len );
				memset(auth_token_b64, 0, b64_len );
					encode_string( (char *)auth_token_raw, (char *)auth_token_b64 );
					sprintf( caDbg, "Authentication token loaded and encoded: (%s) > (%s)\n", auth_token_raw, auth_token_b64 ); XPLMDebugString(caDbg);
	*/

}




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
