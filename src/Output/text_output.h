#ifndef TEXT_OUTPUT_H
#define TEXT_OUTPUT_H

#include "../Models/route.h"
#include "../Graph/graph.h"


void print_route_for_problem(Route* route, Graph* graph, int problem_number,
                             const char* start_time_str, const char* deadline_str);

#endif 