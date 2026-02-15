#ifndef TIME_SCHEDULER_H
#define TIME_SCHEDULER_H

#include "../Utils/common.h"
#include "../Models/route.h"


TimeOfDay add_minutes_to_time(TimeOfDay time, int minutes);


int time_difference_minutes(TimeOfDay later, TimeOfDay earlier);

bool is_time_before(TimeOfDay time1, TimeOfDay time2);


bool is_time_after(TimeOfDay time1, TimeOfDay time2);


TimeOfDay parse_time_string(const char* time_str);

void time_to_string(TimeOfDay time, char* buffer, int buffer_size);

TimeOfDay calculate_arrival_time(Route* route, TimeOfDay start_time);

bool route_meets_deadline(Route* route, TimeOfDay start_time, TimeOfDay deadline);

#endif 