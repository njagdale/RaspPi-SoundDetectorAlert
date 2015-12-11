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
