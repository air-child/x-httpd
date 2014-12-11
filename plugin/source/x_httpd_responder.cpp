//
//  x_httpd_responder.cpp
//  x-httpd
//
//  Created by Ben on 11/12/2014.
//
//

#include "x_httpd_responder.h"




void x_httpd_responder__dref_get::eat( x_httpd_request *request ){
	
	char caTmp[1024]; //used to convert values into a string
	
	printf("response__dref_get.eat()\n");
	
	request->response.setContentType("text/plain");
	
	
	bool valid_request = true;
	
	
	std::string payload = "response__dref_get.eat: ";
	std::string sCmdMode = ""; //define dataref type
	
	size_t max_x = request->requestTokens.size();
	if( max_x < 4 ){
		payload = "Invalid get call: Not enough tokens.";
		valid_request = false;
	}else{
		sCmdMode = request->requestTokens[2];
	}
	
	
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
		#if XPLM200
			printf("XPL mode\n");
		#else
			printf("CLI mode\n");
		#endif
		
		//exec according to mode.
		if( "float" == sCmdMode ){			
			payload = "get float: " + sCmdString;
			
			#if XPLM200
				float tmpVal = XPLMGetDataf( XPLMFindDataRef( sCmdString.c_str() ) );
				sprintf( caTmp, "%f", tmpVal );
				payload += std::string( caTmp );				
			#endif
			
		}else if( "int" == sCmdMode ){
			payload = "get int: " + sCmdString;

			#if XPLM200
				int tmpVal = XPLMGetDatai( XPLMFindDataRef( sCmdString.c_str() ) );
				sprintf( caTmp, "%i", tmpVal );
				payload += std::string( caTmp );				
			#endif
			
		}else if( "double" == sCmdMode ){
			payload = "get double: " + sCmdString;

			#if XPLM200
				double tmpVal = XPLMGetDatad( XPLMFindDataRef( sCmdString.c_str() ) );
				sprintf( caTmp, "%f", tmpVal );
				payload += std::string( caTmp );				
			#endif
			
		}else{
			payload = "Invalid get call: Unknown mode: " + sCmdMode;
			valid_request = false;
		}

		
		
	}else{
		//request has been deemed invalid
	
		//no actions taken.
	}
	
	request->response.setContentBody( payload.c_str() );
	request->response.write();

}





void x_httpd_responder__dref_set::eat( x_httpd_request *request ){


	printf("response__dref_set.eat()\n");
	
	request->response.setContentType("text/plain");
	
	
	bool valid_request = true;
	
	
	std::string payload = "response__dref_set.eat: ";
	std::string sCmdMode = ""; //define dataref type
	std::string sNewValue = ""; //last item on URL
	
	size_t max_x = request->requestTokens.size();
	if( max_x < 5 ){
		payload = "Invalid set call: Not enough tokens.";
		valid_request = false;
	}else{
		sCmdMode = request->requestTokens[2];
		sNewValue = request->requestTokens[ max_x - 1 ];
	}
	
	
	if( valid_request ){
		//reconstruct the command string
		//skip first three tokens (<blank>/cmd/mode/sim/operation/foo)
		//The last token is the value we're sending into the dataref.
		std::string sCmdString = "";
		for( size_t x=3; x < max_x-1; x++ ){			
			sCmdString += request->requestTokens[x];
			if( x < max_x-2 ){
				sCmdString += "/";
			}
		}
		
		//we now have a reconstructed command string
		#if XPLM200
			printf("XPL mode\n");
		#else
			printf("CLI mode\n");
		#endif
		
		//exec according to mode.
		if( "float" == sCmdMode ){			
			payload = "set float: " + sCmdString;
			
			#if XPLM200
				float tmpVal = atof( sNewValue.c_str() );
				XPLMSetDataf( XPLMFindDataRef( sCmdString.c_str() ), tmpVal );
			#endif
			
		}else if( "int" == sCmdMode ){
			payload = "set int: " + sCmdString;

			#if XPLM200
				int tmpVal = atoi( sNewValue.c_str() );
				XPLMSetDatai( XPLMFindDataRef( sCmdString.c_str() ), tmpVal );
			#endif
			
		}else if( "double" == sCmdMode ){
			payload = "set double: " + sCmdString;

			#if XPLM200
				double tmpVal = atof( sNewValue.c_str() );
				XPLMSetDatad( XPLMFindDataRef( sCmdString.c_str() ), tmpVal );
			#endif
			
		}else{
			payload = "Invalid set call: Unknown mode: " + sCmdMode;
			valid_request = false;
		}
		
		
	}else{
		//request has been deemed invalid
	
		//no actions taken.
	}
	
	request->response.setContentBody( payload.c_str() );
	request->response.write();

}








void x_httpd_responder__cmd_handler::eat( x_httpd_request *request ){

	printf("response__cmd_handler.eat()\n");
	
	request->response.setContentType("text/plain");
	
	
	bool valid_request = true;
	
	
	std::string payload = "response__cmd_handler.eat: ";
	std::string sCmdMode = "";
	
	size_t max_x = request->requestTokens.size();
	if( max_x < 4 ){
		payload = "Invalid cmd call: Not enough tokens.";
		valid_request = false;
	}else{
		sCmdMode = request->requestTokens[2];
	}
	
	
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
		#if XPLM200
			printf("XPL mode\n");
		#else
			printf("CLI mode\n");
		#endif
		
		//exec according to mode.
		if( "start" == sCmdMode ){			
			payload = "cmd start: " + sCmdString;
			
			#if XPLM200
				XPLMCommandBegin( XPLMFindCommand( sCmdString.c_str() ) );			
			#endif
			
		}else if( "stop" == sCmdMode ){
			payload = "cmd stop: " + sCmdString;

			#if XPLM200
				XPLMCommandEnd( XPLMFindCommand( sCmdString.c_str() ) );			
			#endif
			
		}else if( "once" == sCmdMode ){
			payload = "cmd once: " + sCmdString;

			#if XPLM200
				XPLMCommandOnce( XPLMFindCommand( sCmdString.c_str() ) );			
			#endif
			
		}else{
			payload = "Invalid cmd call: Unknown mode: " + sCmdMode;
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
