#include "time_scheduler.h"
#include <stdio.h>
#include <string.h>
#include <strings.h> 


TimeOfDay add_minutes_to_time(TimeOfDay time, int minutes) {

    TimeOfDay result = time;
    result.minute += minutes;
    
    
    if (result.minute >= 60) {

        result.hour += result.minute / 60;
        result.minute %= 60;
    }
    
   
    while (result.minute < 0) {

        result.minute += 60;
        result.hour--;
    }
    
   
    result.hour %= 24;

    if (result.hour < 0) {

          result.hour += 24;
    }
    
          return result;
  }


int time_difference_minutes(TimeOfDay later, TimeOfDay earlier) {

    int later_minutes = later.hour * 60 + later.minute;
    int earlier_minutes = earlier.hour * 60 + earlier.minute;
    
    int diff = later_minutes - earlier_minutes;
    
    
    if (diff < 0) {

          diff += 24 * 60;
    }
    
          return diff;
}

 bool is_time_before(TimeOfDay time1, TimeOfDay time2) {

    if (time1.hour < time2.hour){
    
                 return true;
    }

    if (time1.hour > time2.hour) {
    
                 return false;
    }

                 return time1.minute < time2.minute;
}

  bool is_time_after(TimeOfDay time1, TimeOfDay time2) {

           if (time1.hour > time2.hour){
           
                    return true;
           }

          if (time1.hour == time2.hour && time1.minute > time2.minute){ 
    
                    return true;
          }
                    return false;
 }


 TimeOfDay parse_time_string(const char* time_str) {

    TimeOfDay time = {0, 0};

    if (!time_str)
    
          return time;
    
    
    if (sscanf(time_str, "%d:%d", &time.hour, &time.minute) == 2) {

       
        if (strcasestr(time_str, "PM") && time.hour != 12) {

                      time.hour += 12;
        } 
        else if (strcasestr(time_str, "AM") && time.hour == 12) {

                      time.hour = 0;
        }
                      return time;
    }
                      return time;
}


 void time_to_string(TimeOfDay time, char* buffer, int buffer_size) {

              snprintf(buffer, buffer_size, "%02d:%02d", time.hour, time.minute);
 }


  TimeOfDay calculate_arrival_time(Route* route, TimeOfDay start_time) {

    if (!route) {
    
          return start_time;
    }
   
          return add_minutes_to_time(start_time, (int)route->total_time);
}


 bool route_meets_deadline(Route* route, TimeOfDay start_time, TimeOfDay deadline) {

    if (!route) {
     
        return false;
    }
    
    TimeOfDay arrival = calculate_arrival_time(route, start_time);
    
    
    return is_time_before(arrival, deadline) || 
                        (arrival.hour == deadline.hour && arrival.minute == deadline.minute);

 }