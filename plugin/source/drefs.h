/*

This software is made available under the GNU Affero GPL Version 3 license. 
Please see agpl.txt for more information.

Copyright 2005,2013 - Ben Russell, br@x-plugins.com

*/


//
//  drefs.h
//  x-httpd
//
//  Created by Ben Russell on 20/11/13.
//
//

#ifndef __x_httpd__drefs__
#define __x_httpd__drefs__

#include <iostream>

#include "common_sdk.h"




extern XPLMDataRef xpdr_Lat, xpdr_Lon;
extern XPLMDataRef xpdr_AltMSL, xpdr_AltAGL;
extern XPLMDataRef xpdr_X, xpdr_Y, xpdr_Z;
extern XPLMDataRef xpdr_Heading, xpdr_HeadingMag, xpdr_HeadingTrack;
extern XPLMDataRef xpdr_Kias, xpdr_Ktas, xpdr_Kgs;
extern XPLMDataRef xpdr_WindDirection, xpdr_WindSpeed; //wind at aircraft.
extern XPLMDataRef xpdr_Nav1, xpdr_Nav1s;
extern XPLMDataRef xpdr_Com1, xpdr_Com1s;
extern XPLMDataRef xpdr_windAltHigh;
extern XPLMDataRef xpdr_windSpeedHigh;
extern XPLMDataRef xpdr_windDirHigh;
extern XPLMDataRef xpdr_windSheerSpeedHigh;
extern XPLMDataRef xpdr_windSheerDirectionHigh;
extern XPLMDataRef xpdr_windTurbulenceHigh;
extern XPLMDataRef xpdr_windAltMed;
extern XPLMDataRef xpdr_windSpeedMed;
extern XPLMDataRef xpdr_windDirMed;
extern XPLMDataRef xpdr_windSheerSpeedMed;
extern XPLMDataRef xpdr_windSheerDirectionMed;
extern XPLMDataRef xpdr_windTurbulenceMed;
extern XPLMDataRef xpdr_windAltLow;
extern XPLMDataRef xpdr_windSpeedLow;
extern XPLMDataRef xpdr_windDirLow;
extern XPLMDataRef xpdr_windSheerSpeedLow;
extern XPLMDataRef xpdr_windSheerDirectionLow;
extern XPLMDataRef xpdr_windTurbulenceLow;







#endif /* defined(__x_httpd__drefs__) */
