#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "../Utils/common.h"

typedef struct TransportInfo {
    TransportMode mode;
    const char* name;
    double cost_per_km;
    double speed_kmh;
    int schedule_interval_minutes;  
    int start_hour;                 
    int end_hour; 

} TransportInfo;


 TransportInfo get_transport_info(TransportMode mode);

 TransportMode get_transport_mode_from_name(const char* name);

 bool is_transport_available(TransportMode mode, TimeOfDay time);
 TimeOfDay get_next_departure(TransportMode mode, TimeOfDay current_time);
 int calculate_waiting_time(TransportMode mode, TimeOfDay current_time);


 TransportInfo get_transport_info_p5(TransportMode mode); 
 TransportInfo get_transport_info_p6(TransportMode mode);  

 #endif