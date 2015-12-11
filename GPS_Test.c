/*
 * Author:      Nikhil Jagdale
 * Description: This program implements a test harness for GPS.h/c 
 * 
 * Copyright (c) 2015 Nikhil Jagdale
 */ 

#include "GPS.h"
#include <stdio.h>

int main(){

    gps_info gi;

    GetGPSInfo(&gi);

    printf("\nLatutude: %d", gi.latitude);
    printf("\nLongitude: %d", gi.longitude);
    printf("\nDate & Time: %s", asctime(gi.timeinfo));

    return 0;
}
