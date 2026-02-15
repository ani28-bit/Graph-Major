#include "route.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 Route* create_route(Location source, Location dest, TimeOfDay start) {

    Route* route = (Route*)malloc(sizeof(Route));

    if (!route) {

         fprintf(stderr, " Memory allocation failed for Route\n");
         return NULL;
    }
    
    route->source = source;
    route->destination = dest;
    route->start_time = start; 
    route->head = NULL;
    route->tail = NULL;
    route->num_segments = 0;
    route->total_distance = 0.0;
    route->total_cost = 0.0;
    route->total_time = 0.0;
    
    return route;
}

 void add_route_segment(Route* route, int from_id, int to_id,
                         TransportMode mode, double distance,
                          double cost, double duration,
                           TimeOfDay start, TimeOfDay end) {

           if (!route) 
                return;
    
    RouteSegment* segment = (RouteSegment*)malloc(sizeof(RouteSegment));

    if (!segment) {

              fprintf(stderr, "Memory allocation failed for RouteSegment\n");
              return;
    }
    
    segment->from_node_id = from_id;
    segment->to_node_id = to_id;
    segment->mode = mode;
    segment->distance = distance;
    segment->cost = cost;
    segment->duration = duration;
    segment->start_time = start;
    segment->end_time = end;
    segment->next = NULL;
    
    
    if (route->head == NULL) {

            route->head = segment;
            route->tail = segment;
    } 
    else {
            route->tail->next = segment;
            route->tail = segment;
    }
    
       route->num_segments++;
       route->total_distance += distance;
       route->total_cost += cost;
       route->total_time += duration;
       route->arrival_time = end; 
 }

 void calculate_route_totals(Route* route) {

    if (!route || !route->head) 
         return;
    
    double dist = 0, cost = 0, time = 0;
    int count = 0;
    RouteSegment* curr = route->head;
    
    while (curr) {

           dist += curr->distance;
           cost += curr->cost;
           time += curr->duration;
           count++;

        if (curr->next == NULL) {

                 route->arrival_time = curr->end_time;
        }
                 curr = curr->next;
    }
    
           route->total_distance = dist;
           route->total_cost = cost;
           route->total_time = time;
           route->num_segments = count;
  }

 const char* get_mode_name(TransportMode mode) {

    switch (mode) {

        case TRANSPORT_WALK:        return "Walking";
        case TRANSPORT_CAR:         return "Car";
        case TRANSPORT_METRO:       return "Metro";
        case TRANSPORT_BUS_BIKOLPO: return "Bikolpo Bus";
        case TRANSPORT_BUS_UTTARA:  return "Uttara Bus";
        default:                    return "Unknown";
     }
 }

 void format_time(TimeOfDay time, char* buffer, int buffer_size) {

      if (!buffer){ 
          return;
      }
          snprintf(buffer, buffer_size, "%02d:%02d", time.hour, time.minute);
  }

 void free_route(Route* route) {

     if (!route) 

         return;

    RouteSegment* curr = route->head;

    while (curr) {

         RouteSegment* next = curr->next;
         free(curr);
         curr = next;
     }
         free(route);
 }

 void print_route_details(Route* route, Graph* graph) {

    if (!route) {

          printf("No route available.\n");
          return;
    }
    
    printf("\n\n");
    printf("   Detailed Journey Plan (Dhaka Roadmap)   \n");
    printf("\n");
    printf("Source:      (%.6f, %.6f)\n", route->source.latitude, route->source.longitude);
    printf("Destination: (%.6f, %.6f)\n", route->destination.latitude, route->destination.longitude);
    printf("Start Time:  %02d:%02d\n", route->start_time.hour, route->start_time.minute);
    printf("\n");

    int step = 1;
    RouteSegment* seg = route->head;
    
    while (seg) {
        const char* from_name = "Start Point";
        const char* to_name = "End Point";

        if (graph) {

            if (seg->from_node_id >= 0) {

                  Node* n = get_node(graph, seg->from_node_id);

                if (n && n->name[0] != '\0') 

                      from_name = n->name;
            }
            if (seg->to_node_id >= 0) {

                Node* n = get_node(graph, seg->to_node_id);

                if (n && n->name[0] != '\0') 
                        to_name = n->name;
            }
        }

        printf("Step %d: %s -> %s\n", step, from_name, to_name);
        printf("        Mode: %-15s | Distance: %6.2f km | Cost: %6.2f BDT\n", 
               get_mode_name(seg->mode), seg->distance, seg->cost);

        printf("    Time: %02d:%02d - %02d:%02d (Duration: %.0f mins)\n", 
                 seg->start_time.hour, seg->start_time.minute,
                 seg->end_time.hour, seg->end_time.minute,
                 seg->duration);

        printf("n");
        
        seg = seg->next;
        step++;
    }
    
    printf("SUMMARY:\n");

    printf("  Total Distance: %.2f km\n", route->total_distance);
    printf("  Total Cost:     %.2f BDT\n", route->total_cost);
    printf("  Total Time:     %.0f minutes\n", route->total_time);
    printf("  Arrival Time:   %02d:%02d\n", route->arrival_time.hour, route->arrival_time.minute);
    printf("\n");
    
}