#include "location.h"
#include <stdio.h>
#include "../Utils/haversine.h"

Location* create_location(double latitude, double longitude) {

    Location* loc = (Location*)malloc(sizeof(Location));

    if (!loc) {

           fprintf(stderr, "Memory allocation failed for Location\n");
           return NULL;
    }
    
        loc->latitude = latitude;
        loc->longitude = longitude;
    
        return loc;
 }

 Location* copy_location(Location* loc) {

           if (!loc) 
              return NULL;

         return create_location(loc->latitude, loc->longitude);
 }

 bool locations_equal(Location* loc1, Location* loc2, double tolerance) {

         if (!loc1 || !loc2) 
                return false;
    
    
    double distance = haversine_distance(loc1->latitude, loc1->longitude,
                                         loc2->latitude, loc2->longitude);

                   return distance <= tolerance;
  }

  void print_location(Location* loc) {

    if (!loc) {

        printf("NULL location\n");
        return;
    }
         printf("(%.6f, %.6f)", loc->latitude, loc->longitude);
}

 void free_location(Location* loc) {
     if (loc) {

         free(loc);
    }
 }