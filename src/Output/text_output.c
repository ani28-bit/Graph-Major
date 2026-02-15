#include "text_output.h"
#include "../Graph/graph.h"
#include "../Models/route.h"
#include "../Routing/time_scheduler.h"
#include <stdio.h>


static void format_time_12hr(TimeOfDay time, char* buffer, int size) {

    int hour = time.hour;
    const char* period = (hour >= 12) ? "PM" : "AM";
    
    if (hour == 0){

            hour = 12;
    }
    else if (hour > 12){ 

                hour -= 12;
    }
    
    snprintf(buffer, size, "%d:%02d %s", hour, time.minute, period);
}


 static void get_location_string(Graph* graph, int node_id,Location* fallback_loc, 
                                   char* buffer, int size, 
                                     bool is_source, bool is_dest) {

    if (node_id == -1) {

       
        if (is_source) {

                snprintf(buffer, size, "Source (%.6f, %.6f)",fallback_loc->longitude, fallback_loc->latitude);
        } 
        else if (is_dest) {

                    snprintf(buffer, size, "Destination (%.6f, %.6f)",fallback_loc->longitude, fallback_loc->latitude);
        } 

        else {
                    snprintf(buffer, size, "(%.6f, %.6f)",fallback_loc->longitude, fallback_loc->latitude);
        }
    }
    
    else {

             Node* node = get_node(graph, node_id);

        if (node && node->name[0] != '\0') {

           
            snprintf(buffer, size, "%s (%.6f, %.6f)", 
                      node->name, node->longitude, node->latitude);
        } 
        else if (node) {
           
                          snprintf(buffer, size, "(%.6f, %.6f)",node->longitude, node->latitude);
        } 
        else {

                          snprintf(buffer, size, "Unknown");
        }
    }
}


  void print_route_for_problem(Route* route, Graph* graph, int problem_num, 
                                  const char* start_time_str, const char* deadline_str) {

   
       (void)start_time_str;
    
    if (!route) {

        printf("\n[!] No route available.\n");
        return;
    }
    
    char start_time_12hr[32], end_time_12hr[32];
    format_time_12hr(route->start_time, start_time_12hr, sizeof(start_time_12hr));
    format_time_12hr(route->arrival_time, end_time_12hr, sizeof(end_time_12hr));
    
    
    printf("Problem No: %d\n", problem_num);
    printf("Source: (%.6f, %.6f)\n", 
             route->source.longitude, route->source.latitude);
    printf("Destination: (%.6f, %.6f)\n", 
             route->destination.longitude, route->destination.latitude);
    printf("Starting time at source: %s\n", start_time_12hr);
    
    if (deadline_str) {

                printf("Destination reaching time: %s\n\n", deadline_str);
    } 
    
    else {
                printf("Destination reaching time: %s\n\n", end_time_12hr);
    }
    
   
    RouteSegment* seg = route->head;
    bool is_first = true;
    
    while (seg) {

        char seg_start_12hr[32], seg_end_12hr[32];
        format_time_12hr(seg->start_time, seg_start_12hr, sizeof(seg_start_12hr));
        format_time_12hr(seg->end_time, seg_end_12hr, sizeof(seg_end_12hr));
        
        char from_desc[512], to_desc[512];  
        
        
        bool from_is_source = is_first && (seg->from_node_id == -1);
        bool to_is_dest = (seg->next == NULL) && (seg->to_node_id == -1);
        
        get_location_string(graph, seg->from_node_id, 
                             from_is_source ? &route->source : NULL,
                              from_desc, sizeof(from_desc), 
                                from_is_source, false);
        
        get_location_string(graph, seg->to_node_id,
                              to_is_dest ? &route->destination : NULL,
                                to_desc, sizeof(to_desc), 
                                  false, to_is_dest);
        
        
         const char* action;
          const char* mode_name;
          if (seg->mode == TRANSPORT_WALK) {

            action = "Walk";
            mode_name = ""; 

        } 
        else {

                 action = "Ride";
                 mode_name = get_mode_name(seg->mode);
        }
        
        
        if (seg->mode == TRANSPORT_WALK) {

            printf("%s - %s, Cost: ৳%.2f: %s from %s to %s.\n",
                       seg_start_12hr, seg_end_12hr, seg->cost,
                        action, from_desc, to_desc);
        } 
        else {

                  printf("%s - %s, Cost: ৳%.2f: %s %s from %s to %s.\n",
                     seg_start_12hr, seg_end_12hr, seg->cost,
                       action, mode_name, from_desc, to_desc);
        }
        
        is_first = false;
        seg = seg->next;
    }
    
         printf("\n");
}