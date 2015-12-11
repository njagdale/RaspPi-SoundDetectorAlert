/*
 * Author:      Nikhil Jagdale
 * Description: This program provides dummy GPS co-odrindates generated ramdomly 
 *              along with the current system date and time. Information is packed 
 *              in a struct for the client to further process
 * 
 * Copyright (c) 2015 Nikhil Jagdale
 */ 

#include "GPS.h"

static int GetRandomLatitide(){
    srand(time(NULL));
    return rand()%90;
}

static int GetRandomLongitude(){
    //srand(time(NULL));
    return rand()%180;
}

void GetGPSInfo(gps_info* data){
    
    time_t rawtime;
    time(&rawtime);

    data->latitude = GetRandomLatitide();
    data->longitude = GetRandomLongitude();
    data->timeinfo = localtime(&rawtime);    
    
}
