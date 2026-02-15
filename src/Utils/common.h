#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>


#define MAX_LINE_LENGTH 8192 

#define MAX_NAME_LENGTH 256
#define MAX_COORDINATES 1000
#define EARTH_RADIUS_KM 6371.0


#define WALKING_SPEED_KMH 2.0      
#define CAR_SPEED_KMH_FIXED 30.0   
#define CAR_SPEED_P6 20.0          
#define METRO_SPEED_P6 15.0        
#define BIKOLPO_BUS_SPEED_P6 10.0   
#define UTTARA_BUS_SPEED_P6 12.0    
#define ALL_VEHICLE_SPEED_P5 10.0  


#define COST_CAR 20.0              
#define COST_METRO 5.0             
#define COST_BUS_BIKOLPO 7.0       
#define COST_BUS_UTTARA 10.0       


#define FREQ_P4_P5 15              
#define FREQ_METRO_P6 5            
#define FREQ_BIKOLPO_P6 20         
#define FREQ_UTTARA_P6 10          


typedef enum {

    TRANSPORT_WALK,
    TRANSPORT_CAR,
    TRANSPORT_METRO,
    TRANSPORT_BUS_BIKOLPO,
    TRANSPORT_BUS_UTTARA
} TransportMode;

typedef enum {

    NODE_GENERIC = 0,
    NODE_ROAD,              
    NODE_METRO_STATION,     
    NODE_BUS_STOP           
} NodeType;

typedef enum {

    OPTIMIZE_DISTANCE,
    OPTIMIZE_COST,
    OPTIMIZE_TIME
} OptimizationCriterion;

typedef struct {

    double latitude;
    double longitude;
} Location;

typedef struct {

    int hour;      
    int minute;  
} TimeOfDay;


static inline int to_total_minutes(TimeOfDay t) {

       return (t.hour * 60) + t.minute;
}

static inline TimeOfDay from_total_minutes(int total_minutes) {

    TimeOfDay t;
    t.hour = total_minutes / 60;
    t.minute = total_minutes % 60;
    return t;
}


static inline bool is_before_or_equal(TimeOfDay t1, TimeOfDay t2) {

    int mins1 = to_total_minutes(t1);
    int mins2 = to_total_minutes(t2);
    return mins1 <= mins2;
}


static inline int min_int(int a, int b) { 
    
        return (a < b) ? a : b;
 }
static inline int max_int(int a, int b) { 

        return (a > b) ? a : b;
 }

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DEG_TO_RAD(deg) ((deg) * M_PI / 180.0)


#define SUCCESS 0
#define ERROR_FILE_NOT_FOUND -1
#define ERROR_INVALID_INPUT -2
#define ERROR_MEMORY_ALLOCATION -3
#define ERROR_NO_PATH_FOUND -4

#endif