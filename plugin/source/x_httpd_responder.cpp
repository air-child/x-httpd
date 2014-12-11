//
//  x_httpd_responder.cpp
//  x-httpd
//
//  Created by Ben on 11/12/2014.
//
//

#include "x_httpd_responder.h"




void x_httpd_responder__dref_get::eat( x_httpd_request *request ){

}

void x_httpd_responder__dref_set::eat( x_httpd_request *request ){

}




void x_httpd_responder__cmd_handler::eat( x_httpd_request *request ){

	printf("response__cmd_handler.eat()\n");
	
	request->response.setContentType("text/plain");
	
	
	bool valid_request = true;
	
	
	std::string payload = "response__cmd_handler.eat: ";
	
	size_t max_x = request->requestTokens.size();
	if( max_x < 4 ){
		payload = "Invalid cmd call: Not enough tokens.";
		valid_request = false;
	}
	
	std::string sCmdMode = request->requestTokens[2];
	
	
	if( valid_request ){
		//reconstruct the command string
		//skip first three tokens (<blank>/cmd/mode/sim/operation/foo)
		std::string sCmdString = "";
		for( size_t x=3; x < max_x; x++ ){			
			sCmdString += request->requestTokens[x];
			if( x < max_x-1 ){
				sCmdString += "/";
			}
		}
		
		//we now have a reconstructed command string
		
		//exec according to mode.
		if( "start" == sCmdMode ){			
			payload = "cmd start: " + sCmdString;
			
		}else if( "stop" == sCmdMode ){
			payload = "cmd stop: " + sCmdString;
			
		}else if( "once" == sCmdMode ){
			payload = "cmd once: " + sCmdString;
			
		}else{
			payload = "Invalid cmd call: Unknown mode.";
			valid_request = false;
		}

		
		
	}else{
		//request has been deemed invalid
	
		//no actions taken.
	}
	
	request->response.setContentBody( payload.c_str() );
	request->response.write();

}




//eof
