#include <stdio.h>
#include <math.h> 
#include "haversine.h"

double haversine_distance(double lat1, double lon1, double lat2, double lon2) {
    
    if (lat1 == lat2 && lon1 == lon2) 

          return 0.0;

    double lat1_rad = DEG_TO_RAD(lat1);
    double lon1_rad = DEG_TO_RAD(lon1);
    double lat2_rad = DEG_TO_RAD(lat2);
    double lon2_rad = DEG_TO_RAD(lon2);
    
    double dlat = lat2_rad - lat1_rad;
    double dlon = lon2_rad - lon1_rad;
    
    double a = sin(dlat / 2.0) * sin(dlat / 2.0) +
               cos(lat1_rad) * cos(lat2_rad) *
               sin(dlon / 2.0) * sin(dlon / 2.0);
    
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    
    return EARTH_RADIUS_KM * c;
}

double location_distance(Location* loc1, Location* loc2) {

    if (!loc1 || !loc2) {
         
           return -1.0; 
    }
    
    return haversine_distance(loc1->latitude, loc1->longitude,
                              loc2->latitude, loc2->longitude);
}