/*

This software is made available under the GNU Affero GPL Version 3 license. 
Please see agpl.txt for more information.

Copyright 2005,2013 - Ben Russell, br@x-plugins.com

*/


//
//  drefs.cpp
//  x-httpd
//
//  Created by Ben Russell on 20/11/13.
//
//

#include "drefs.h"




XPLMDataRef xpdr_Lat, xpdr_Lon;
XPLMDataRef xpdr_AltMSL, xpdr_AltAGL;
XPLMDataRef xpdr_X, xpdr_Y, xpdr_Z;

XPLMDataRef xpdr_Heading, xpdr_HeadingMag, xpdr_HeadingTrack;

XPLMDataRef xpdr_Kias, xpdr_Ktas, xpdr_Kgs;

XPLMDataRef xpdr_WindDirection, xpdr_WindSpeed; //wind at aircraft.

XPLMDataRef xpdr_Nav1, xpdr_Nav1s;
XPLMDataRef xpdr_Com1, xpdr_Com1s;



XPLMDataRef xpdr_windAltHigh;
XPLMDataRef xpdr_windSpeedHigh;
XPLMDataRef xpdr_windDirHigh;
XPLMDataRef xpdr_windSheerSpeedHigh;
XPLMDataRef xpdr_windSheerDirectionHigh;
XPLMDataRef xpdr_windTurbulenceHigh;

XPLMDataRef xpdr_windAltMed;
XPLMDataRef xpdr_windSpeedMed;
XPLMDataRef xpdr_windDirMed;
XPLMDataRef xpdr_windSheerSpeedMed;
XPLMDataRef xpdr_windSheerDirectionMed;
XPLMDataRef xpdr_windTurbulenceMed;

XPLMDataRef xpdr_windAltLow;
XPLMDataRef xpdr_windSpeedLow;
XPLMDataRef xpdr_windDirLow;
XPLMDataRef xpdr_windSheerSpeedLow;
XPLMDataRef xpdr_windSheerDirectionLow;
XPLMDataRef xpdr_windTurbulenceLow;

/*
XPLMDataRef xpdr_;
XPLMDataRef xpdr_;
XPLMDataRef xpdr_;
XPLMDataRef xpdr_;
XPLMDataRef xpdr_;
*/
