//
//  main.cpp
//  x-httpd-cli
//
//  Created by Ben on 7/12/2014.
//
//

#include <iostream>


#include "x_httpd.h"


int main(int argc, const char * argv[])
{

	// insert code here...
	std::cout << "x-httpd-cli 14.12.07.1819\n";
	std::cout << "\n";
	
	
	std::cout << "  spawning server..\n";
	x_httpd *httpd = new x_httpd( 1312 );
	
	
	std::cout << "  running..\n";
	httpd->run();
	
	
	std::cout << "finished.\n";
    return 0;
}

