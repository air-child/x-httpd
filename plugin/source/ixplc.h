//
//  ixplc.h
//  x-httpd
//
//  Created by Ben Russell on 22/11/13.
//
//

#ifndef __x_httpd__ixplc__
#define __x_httpd__ixplc__

#include <iostream>


struct ixplc_packet{
	intptr_t size;
	intptr_t packet_id;
	intptr_t message_type;
	void* blob;
};



class ixplc{
	public:
		void encode();
		void decode();
		



};



#endif /* defined(__x_httpd__ixplc__) */
