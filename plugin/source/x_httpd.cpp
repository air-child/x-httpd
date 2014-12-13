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






x_httpd::x_httpd( const int port, const char* web_root_folder ){

	this->sock = 0; //setup by socket init function

	//options to control the server.
	this->bAllowRemoteConnections = 1;
	this->bRequirePassword = 0;
	this->bLogDebugToConsole = 1;
	
	//default value for username and password for auth
	this->sAuthTokenRaw = "username:password";
	
	this->sWebFolder = std::string( web_root_folder );
	printf("Content root:(%s)\n", this->sWebFolder.c_str());


	printf("Starting hpt timer..\n");
	this->hpt.start();


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


void x_httpd::run_cli(){

	for(;;){
	
		this->run_slice( 5 * 1000 ); //N ms
		
		//If we do NOT usleep then the network stack seems unable to feed us a large enough packet of data to read and parse as a valid request.
		usleep( 1000 ); //1ms sleep between run slices
	
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

			
			double hptStart = this->hpt.getElapsedTimeInMicroSec();


			int c = 0; //client socket, populated by accept(...)
			int clientCount = 0;
			do{
				clientCount++;
			
				if( clientCount >= 10 ){ 
					printf("!!! breaking accept loop: >= 10 clients processed.\n");
					break; 
				} //we want to give x-plane priority, 10 clients per frame event.
				
				
				if( (this->hpt.getElapsedTimeInMicroSec() - hptStart) > (double)time_usec ){
					printf("!!! breaking accept loop: time limit.\n");
					break;
				} //we hit our max time limit as specified in function argument.
				
			
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
						printf("\n");
						printf("accepted client: %i\n", c);
						
							char remoteAddress[32];
							strcpy( remoteAddress, inet_ntoa(from.sin_addr) );
							
							sprintf( caDbg, "x-httpd: request from: %s\n", remoteAddress );
							printf( "%s", caDbg );
							//XPLMDebugString(caDbg);
							
							
							//if(true){
							if( (strcmp( "127.0.0.1", remoteAddress ) == 0) || bAllowRemoteConnections ){
						
								//ntohl( &(from.sin_addr) );
								if( bLogDebugToConsole ){
									printf( "*** Connection from: %s\n", remoteAddress );
								}
						

								//create new request object, passing our client socket and our auth-token
								x_httpd_request req = x_httpd_request( c, "auth_token_b64" );
								req.setWebRoot( sWebFolder.c_str() );
								

										//make choices about what content to serve
										req.processRequest();
								
								
							}else{
								//remote connections are NOT allowed.
								printf( "x-httpd: Access Denied: allow_remote: %i\n", bAllowRemoteConnections );
								close(c); //drop client without writing any data.
								
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


		int server_port_number = 1312;

		struct sockaddr_in sin;
		
		sin.sin_family=AF_INET;
		sin.sin_port=htons(server_port_number);
			//net_aton("127.0.0.1", &(sin.sin_addr));
		sin.sin_addr.s_addr= htonl(INADDR_ANY);

		this->sock = socket( AF_INET, SOCK_STREAM, 0 );
		
		int sflag = 1;
		setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, (char*)&sflag, sizeof(int) );
		
		int sflags = fcntl( sock, F_GETFL );
		//fcntl( sock, F_SETFL, O_NONBLOCK );
		fcntl( sock, F_SETFL, sflags | O_NONBLOCK );
		
		if( bind( sock, (const sockaddr*)&sin, sizeof(sin) ) != 0 ){
			printf("x-httpd: Failed to bind server socket to port: %i\n", server_port_number);
			return;
		}else{
			listen( sock, 5 );
			printf("Socket listening on port %i.\n", server_port_number);
		}
	

}













//eof
