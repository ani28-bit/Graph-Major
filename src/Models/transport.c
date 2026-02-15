#include "transport.h"
#include <string.h>
#include <stdio.h>


TransportInfo get_transport_info(TransportMode mode) {

    TransportInfo info;
    info.mode = mode;
    
    switch (mode) {

        case TRANSPORT_WALK:
            info.name = "Walking";
            info.cost_per_km = 0.0;
            info.speed_kmh = WALKING_SPEED_KMH; 
            info.schedule_interval_minutes = 0;
            info.start_hour = 0;
            info.end_hour = 24;
            break;
            
        case TRANSPORT_CAR:
            info.name = "Car";
            info.cost_per_km = COST_CAR;  
            info.speed_kmh = CAR_SPEED_KMH_FIXED;  
            info.schedule_interval_minutes = 0;
            info.start_hour = 0;
            info.end_hour = 24;
            break;
            
        case TRANSPORT_METRO:
            info.name = "Metro";
            info.cost_per_km = COST_METRO;  
            info.speed_kmh = CAR_SPEED_KMH_FIXED;  
            info.schedule_interval_minutes = FREQ_P4_P5;  
            info.start_hour = 6;  
            info.end_hour = 23;    
            break;
            
        case TRANSPORT_BUS_BIKOLPO:
            info.name = "Bikolpo Bus";
            info.cost_per_km = COST_BUS_BIKOLPO;  
            info.speed_kmh = CAR_SPEED_KMH_FIXED; 
            info.schedule_interval_minutes = FREQ_P4_P5;  
            info.start_hour = 6;   
            info.end_hour = 23;    
            break;
            
        case TRANSPORT_BUS_UTTARA:
            info.name = "Uttara Bus";
            info.cost_per_km = COST_BUS_UTTARA;  
            info.speed_kmh = CAR_SPEED_KMH_FIXED;  
            info.schedule_interval_minutes = FREQ_P4_P5;  
            info.start_hour = 6;   
            info.end_hour = 23;    
            break;
            
        default:
            info.name = "Unknown";
            info.cost_per_km = 0.0;
            info.speed_kmh = CAR_SPEED_KMH_FIXED;
            info.schedule_interval_minutes = 0;
            info.start_hour = 0;
            info.end_hour = 24;
            break;
    }
    
    return info;
}


TransportInfo get_transport_info_p5(TransportMode mode) {

    TransportInfo info = get_transport_info(mode);
    
   
    if (mode != TRANSPORT_WALK) {

                info.speed_kmh = ALL_VEHICLE_SPEED_P5; 
    }
    
                return info;
 }


 TransportInfo get_transport_info_p6(TransportMode mode) {

    TransportInfo info;
    info.mode = mode;
    
    switch (mode) {

        case TRANSPORT_WALK:
            info.name = "Walking";
            info.cost_per_km = 0.0;
            info.speed_kmh = WALKING_SPEED_KMH;  
            info.schedule_interval_minutes = 0;
            info.start_hour = 0;
            info.end_hour = 24;
            break;
            
        case TRANSPORT_CAR:
            info.name = "Car";
            info.cost_per_km = COST_CAR;  
            info.speed_kmh = CAR_SPEED_P6; 
            info.schedule_interval_minutes = 0;
            info.start_hour = 0;
            info.end_hour = 24;
            break;
            
        case TRANSPORT_METRO:
            info.name = "Metro";
            info.cost_per_km = COST_METRO; 
            info.speed_kmh = METRO_SPEED_P6;  
            info.schedule_interval_minutes = FREQ_METRO_P6;  
            info.start_hour = 1;  
            info.end_hour = 23;    
            break;
            
        case TRANSPORT_BUS_BIKOLPO:
            info.name = "Bikolpo Bus";
            info.cost_per_km = COST_BUS_BIKOLPO;  
            info.speed_kmh = BIKOLPO_BUS_SPEED_P6;  
            info.schedule_interval_minutes = FREQ_BIKOLPO_P6; 
            info.start_hour = 7;  
            info.end_hour = 22;   
            break;
            
        case TRANSPORT_BUS_UTTARA:
            info.name = "Uttara Bus";
            info.cost_per_km = COST_BUS_UTTARA; 
            info.speed_kmh = UTTARA_BUS_SPEED_P6;  
            info.schedule_interval_minutes = FREQ_UTTARA_P6;  
            info.start_hour = 6; 
            info.end_hour = 23;   
            break;
            
        default:
            info.name = "Unknown";
            info.cost_per_km = 0.0;
            info.speed_kmh = CAR_SPEED_P6;
            info.schedule_interval_minutes = 0;
            info.start_hour = 0;
            info.end_hour = 24;
            break;
     }
    
    return info;
 }


 bool is_transport_available(TransportMode mode, TimeOfDay time) {

    TransportInfo info = get_transport_info(mode);
    
  
    if (mode == TRANSPORT_WALK || mode == TRANSPORT_CAR) {

           return true;
    }
    
   
    
           return (time.hour >= info.start_hour && time.hour < info.end_hour);
 }


 
 int calculate_waiting_time(TransportMode mode, TimeOfDay current_time) {

    TransportInfo info = get_transport_info(mode);
    
    
    if (info.schedule_interval_minutes <= 0) {

          return 0;
    }
    
   
    if (!is_transport_available(mode, current_time)) {

           return 9999;  
    }
    
    int total_minutes = current_time.hour * 60 + current_time.minute;
    int service_start = info.start_hour * 60;
    int minutes_since_start = total_minutes - service_start;
    
    
    int wait = info.schedule_interval_minutes - (minutes_since_start % info.schedule_interval_minutes);
    
   
    if (wait == info.schedule_interval_minutes) {

           return 0;
    }
    
           return wait;
 }


  TimeOfDay get_next_departure(TransportMode mode, TimeOfDay current_time) {

    int wait = calculate_waiting_time(mode, current_time);
    
    TimeOfDay next = current_time;
    next.minute += wait;
    
    if (next.minute >= 60) {

        next.hour = (next.hour + next.minute / 60) % 24;
        next.minute %= 60;
    }
    
        return next;
 }

 TransportMode get_transport_mode_from_name(const char* name) {

        if (!name) 

            return TRANSPORT_WALK;
    
        if (strcasecmp(name, "Car") == 0 || strcasecmp(name, "DhakaStreet") == 0) {

               return TRANSPORT_CAR;
          } 
    else if (strcasecmp(name, "Metro") == 0 || strcasecmp(name, "DhakaMetroRail") == 0) {

             return TRANSPORT_METRO;
         }
     else if (strcasecmp(name, "BikolpoBus") == 0 || strcasecmp(name, "Bikolpo") == 0) {

                   return TRANSPORT_BUS_BIKOLPO;
         } 
    else if (strcasecmp(name, "UttaraBus") == 0 || strcasecmp(name, "Uttara") == 0) {

                   return TRANSPORT_BUS_UTTARA;
      } 
    else if (strcasecmp(name, "Walk") == 0 || strcasecmp(name, "Walking") == 0) {

                  return TRANSPORT_WALK;
     }
    
   
                   return TRANSPORT_CAR;
 }