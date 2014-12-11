//
//  x_httpd_responder.cpp
//  x-httpd
//
//  Created by Ben on 11/12/2014.
//
//

#include "x_httpd_responder.h"




void x_httpd_responder__dref_get::write( x_httpd_request *request ){

}

void x_httpd_responder__dref_set::write( x_httpd_request *request ){

}




void x_httpd_responder__cmd_handler::write( x_httpd_request *request ){

	printf("response__cmd_handler.write()\n");
	
	request->response.setContentType("text/plain");
	request->response.setContentBody( "response__cmd_handler.write()" );
	request->response.write();

}




//eof
