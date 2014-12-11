//
//  x_httpd_responder.h
//  x-httpd
//
//  Created by Ben on 11/12/2014.
//
//

#ifndef __x_httpd__x_httpd_responder__
#define __x_httpd__x_httpd_responder__

#include <iostream>

#include <map>
#include <string>
#include <vector>



#include "x_httpd_request.h"
#include "x_httpd_response.h"



/**
Superclass for registered content responders to inherit and modify.
Handles logic for handling things like: /get/... /set/... /cmd/... etc, etc.
*/
class x_httpd_responder{
	protected:
		//Nothing in super class, Request instance state is passed as arguments to eat()
		
	public:
		//x_httpd_responder();
		//~x_httpd_responder();
		
		//this function will be the only thing called by x_httpd_request::processRequest()
		//arguments are passed here as the module will be instantiated for the life of the server but it must be able to
		//process many different requests during its lifetime.
		//Yes, there are better ways to do this and instantiate only when needed but that can wait for later. Bigger fish to fry.
		virtual void eat( x_httpd_request *request ){
			//blank!
		};

};


class x_httpd_responder__dref_access : public x_httpd_responder{
	protected:
		//dataref handle pool?
		
	public:
		//inherited from super
		//virtual void eat( x_httpd_request &req, x_httpd_response &resp );
};


class x_httpd_responder__dref_get : public x_httpd_responder__dref_access{
	protected:
		//superclass dref pool
	public:
		virtual void eat( x_httpd_request *request );
};

class x_httpd_responder__dref_set : public x_httpd_responder__dref_access{
	protected:
		//superclass dref pool
	public:
		virtual void eat( x_httpd_request *request );
};




class x_httpd_responder__cmd_handler : public x_httpd_responder{
	protected:
		//command handle pool?
		//state pool so we know which items are currently in cmd_begin mode.
		// cmd_begin, cmd_hold, cmd_end
		//cmd_hold will be automatically fired by x-plane after 2(?) subsequent graphics frames where a command is in the state of cmd_begin
		//incredibly short detection and can be quite annoying.
		
	public:
		virtual void eat( x_httpd_request *request );
};


#endif /* defined(__x_httpd__x_httpd_responder__) */
