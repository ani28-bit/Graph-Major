#include "cost_calculator.h"
#include "../Models/transport.h"
#include <stdio.h>
#include <math.h>


double calculate_cost(double distance_km, TransportMode mode) {

          TransportInfo info = get_transport_info(mode);
          return distance_km * info.cost_per_km;
}


double calculate_travel_time(double distance_km, TransportMode mode, TimeOfDay current_time) {

    TransportInfo info = get_transport_info(mode);
    
    if (info.speed_kmh <= 0) 
             return 0.0;
    
   
    double travel_time_mins = (distance_km / info.speed_kmh) * 60.0;
    
    
    int waiting_mins = 0;
    if (mode == TRANSPORT_METRO || mode == TRANSPORT_BUS_BIKOLPO || mode == TRANSPORT_BUS_UTTARA) {

                           waiting_mins = calculate_waiting_time(mode, current_time);
     }
    
                           return travel_time_mins + (double)waiting_mins;
 }

 double calculate_route_cost(Route* route) {

    if (!route || !route->head) 
     
               return 0.0;
    
    double total_cost = 0.0;
    RouteSegment* current = route->head;
    
    while (current) {

        
        if (current->cost <= 0) {

                      current->cost = calculate_cost(current->distance, current->mode);
        }

                     total_cost += current->cost;
                     current = current->next;
    }
                      return total_cost;
}


   double calculate_route_time(Route* route) {

        if (!route || !route->head)
        
                   return 0.0;
    
    double total_accumulated_mins = 0.0;
    TimeOfDay current_clock = route->start_time;
    RouteSegment* current = route->head;
    
    while (current) {
       
        
        double seg_duration = calculate_travel_time(current->distance, current->mode, current_clock);
        
        total_accumulated_mins += seg_duration;
        
       
        
        int total_mins = current_clock.hour * 60 + current_clock.minute + (int)ceil(seg_duration);
        current_clock.hour = (total_mins / 60) % 24;
        current_clock.minute = total_mins % 60;
        
        current = current->next;
     }
        return total_accumulated_mins;
 }


 double calculate_route_distance(Route* route) {

         if (!route || !route->head) 
         
                   return 0.0;
    
    double total_dist = 0.0;
    RouteSegment* current = route->head;
    
    while (current) {

              total_dist += current->distance;
               current = current->next;
    }
               return total_dist;
}