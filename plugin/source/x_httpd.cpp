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


#include "x_httpd_request.h"


x_httpd::x_httpd( int port ){

	this->sock = 0; //setup by socket init function

	//options to control the server.
	this->bAllowRemoteConnections = 1;
	this->bRequirePassword = 0;
	this->bLogDebugToConsole = 1;
	
	//default value for username and password for auth
	this->sAuthTokenRaw = "username:password";


}

x_httpd::~x_httpd(){

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
		usleep( 100 );
	
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
							printf( "%s", caDbg );
							//XPLMDebugString(caDbg);
							
							
							//if(true){
							if( (strcmp( "127.0.0.1", remoteAddress ) == 0) || bAllowRemoteConnections ){
						
								//ntohl( &(from.sin_addr) );
								if( bLogDebugToConsole ){
									printf( "*** Connection from: %s\n", remoteAddress );
								}
						
								//processConnection(c);
								
								x_httpd_request req = x_httpd_request( c, "auth_token_b64" );
								
								
							}else{
							
								sprintf( caDbg, "x-httpd: Access Denied: allow_remote: %i\n", bAllowRemoteConnections );
								//XPLMDebugString(caDbg);
								printf( "%s", caDbg );
							
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













//eof
