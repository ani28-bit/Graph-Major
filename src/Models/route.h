#ifndef ROUTE_H
#define ROUTE_H

#include "../Utils/common.h"
#include "../Graph/graph.h"


typedef struct RouteSegment {

    int from_node_id;
    int to_node_id;
    TransportMode mode;
    double distance;
    double cost;
    double duration;
    TimeOfDay start_time;
    TimeOfDay end_time;
    struct RouteSegment* next;
} RouteSegment;


typedef struct Route {

    RouteSegment* head;
    RouteSegment* tail;
    int num_segments;
    double total_distance;
    double total_cost;
    double total_time;
    Location source;
    Location destination;
    TimeOfDay start_time;
    TimeOfDay arrival_time;
} Route;


Route* create_route(Location source, Location dest, TimeOfDay start);
void add_route_segment(Route* route, int from_id, int to_id,
                       TransportMode mode, double distance,
                       double cost, double duration,
                       TimeOfDay start, TimeOfDay end);

void calculate_route_totals(Route* route);
const char* get_mode_name(TransportMode mode);

void format_time(TimeOfDay time, char* buffer, int buffer_size);
void free_route(Route* route);


void print_route_details(Route* route, Graph* graph);

#endif