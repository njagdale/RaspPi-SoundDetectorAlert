/*
 * Author:      Nikhil Jagdale
 * Description: API for getting dummy GPS Coordinates 
 * 
 * Copyright (c) 2015 Nikhil Jagdale
*/

#ifndef __GPS_H__
#define __GPS_H__

#include <time.h>

typedef struct gps_info{
    
    int latitude;
    int longitude;
    struct tm* timeinfo;    
      
}gps_info;

void GetGPSInfo(gps_info*);

#endif
