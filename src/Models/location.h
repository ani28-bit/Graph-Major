#ifndef LOCATION_H
#define LOCATION_H

#include <stdbool.h>
#include <stdlib.h>
#include "../Utils/common.h"


Location* create_location(double latitude, double longitude);


Location* copy_location(Location* loc);


bool locations_equal(Location* loc1, Location* loc2, double tolerance);

void print_location(Location* loc);

void free_location(Location* loc);

#endif 