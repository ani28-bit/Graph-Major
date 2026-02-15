#ifndef HAVERSINE_H
#define HAVERSINE_H

#include "common.h"

double haversine_distance(double lat1, double lon1, double lat2, double lon2);


double location_distance(Location* loc1, Location* loc2);

#endif 