#ifndef COST_CALCULATOR_H
#define COST_CALCULATOR_H

#include "../Utils/common.h"
#include "../Models/route.h"


 double calculate_cost(double distance_km, TransportMode mode);


 double calculate_travel_time(double distance_km, TransportMode mode, TimeOfDay current_time);


 double calculate_route_cost(Route* route);

 double calculate_route_time(Route* route);


 double calculate_route_distance(Route* route);

 #endif 