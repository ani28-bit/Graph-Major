#include "route_finder.h"
#include "../Graph/graph.h"
#include "../Algorithms/dijkstra.h"
#include "time_scheduler.h"
#include "cost_calculator.h"
#include "../Utils/haversine.h"
#include "../Models/transport.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


 Route* path_to_route(Graph* graph, PathResult* path_result,
                       Location source, Location dest,
                        TimeOfDay start_time,
                         int allowed_modes) {

    if (!graph || !path_result || !path_result->found)
    
                   return NULL;
    
    Route* route = create_route(source, dest, start_time);

    if (!route) 
         return NULL;
    
    TimeOfDay current_time = start_time;
    
    
    if (path_result->path_length > 0) {

        Node* first_node = get_node(graph, path_result->path[0]);

        if (first_node) {

            double walk_dist = haversine_distance(source.latitude, source.longitude,
                                                    first_node->latitude, first_node->longitude);
            
            if (walk_dist > 0.005) {

                double walk_time = calculate_travel_time(walk_dist, TRANSPORT_WALK, current_time);
                TimeOfDay arrival_time = add_minutes_to_time(current_time, (int)ceil(walk_time));

                add_route_segment(route, -1, first_node->id, TRANSPORT_WALK,
                                    walk_dist, 0.0, walk_time, current_time, arrival_time);

                                  current_time = arrival_time;
            }
        }
    }
    
   
    for (int i = 0; i < path_result->path_length - 1; i++) {

        int from_id = path_result->path[i];
        int to_id = path_result->path[i + 1];
        
        Edge* edge = get_neighbors(graph, from_id);
        Edge* best_edge = NULL;
        double min_val = INFINITY;

        while (edge) {
            if (edge->to_node_id == to_id && ((1 << edge->mode) & allowed_modes)) {

               
                if (edge->distance < min_val) {

                    min_val = edge->distance;
                    best_edge = edge;
                }
            }
                    edge = edge->next;
        }

        if (best_edge) {

            
            double total_duration = calculate_travel_time(best_edge->distance, best_edge->mode, current_time);
            
            
            int wait = calculate_waiting_time(best_edge->mode, current_time);
            TimeOfDay departure_time = add_minutes_to_time(current_time, wait);
            
            
            TransportInfo info = get_transport_info(best_edge->mode);
            double travel_time = (best_edge->distance / info.speed_kmh) * 60.0;
            TimeOfDay arrival_time = add_minutes_to_time(departure_time, (int)ceil(travel_time));
            
            double cost = calculate_cost(best_edge->distance, best_edge->mode);

            add_route_segment(route, from_id, to_id, best_edge->mode,
                                best_edge->distance, cost, total_duration,
                                current_time, arrival_time);
            
                               current_time = arrival_time;
        }
    }
    
    
    if (path_result->path_length > 0) {

        Node* last_node = get_node(graph, path_result->path[path_result->path_length - 1]);

        if (last_node) {

               double walk_dist = haversine_distance(last_node->latitude, last_node->longitude,
                                                      dest.latitude, dest.longitude);
            
            if (walk_dist > 0.005) {

                double walk_time = calculate_travel_time(walk_dist, TRANSPORT_WALK, current_time);
                TimeOfDay arrival_time = add_minutes_to_time(current_time, (int)ceil(walk_time));

                add_route_segment(route, last_node->id, -1, TRANSPORT_WALK,
                                   walk_dist, 0.0, walk_time, current_time, arrival_time);

                                 route->arrival_time = arrival_time;
            } 
            else {

                                 route->arrival_time = current_time;
            }
        }
    }
    
                                calculate_route_totals(route);
                                return route;
 }


  Route* solve_problem_1(Graph* graph, Location source, Location dest) {

    Node* s = find_nearest_node(graph, source.latitude, source.longitude, NODE_ROAD);
    Node* d = find_nearest_node(graph, dest.latitude, dest.longitude, NODE_ROAD);
    
    if (!s || !d) {

        printf("Could not find nearest road nodes for source/destination.\n");
        return NULL;
    }

    TimeOfDay t_start = {8, 0};  
    int allowed = (1 << TRANSPORT_CAR);
    
    PathResult* res = dijkstra(graph, s->id, d->id, OPTIMIZE_DISTANCE, allowed, t_start); 

    if (!res || !res->found) {

        printf("No path found for Problem 1.\n");

        if (res){
        
             free_path_result(res);
        }
             return NULL;
    }
    
           Route* route = path_to_route(graph, res, source, dest, t_start, allowed);
           free_path_result(res);
           return route;
 }


 Route* solve_problem_2(Graph* graph, Location source, Location dest) {

    Node* s = find_nearest_node(graph, source.latitude, source.longitude, NODE_GENERIC);
    Node* d = find_nearest_node(graph, dest.latitude, dest.longitude, NODE_GENERIC);
    
    if (!s || !d) {

           printf("Could not find nearest nodes for source/destination.\n");
           return NULL;
    }

    TimeOfDay t_start = {8, 0};
    int allowed = (1 << TRANSPORT_CAR) | (1 << TRANSPORT_METRO) | (1 << TRANSPORT_WALK);
    
    PathResult* res = dijkstra(graph, s->id, d->id, OPTIMIZE_COST, allowed, t_start);

    if (!res || !res->found) {

        printf("No path found for Problem 2.\n");

        if (res) {
            free_path_result(res);
        }
            return NULL;
    }
    
         Route* route = path_to_route(graph, res, source, dest, t_start, allowed);
         free_path_result(res);
         return route;
 }


 Route* solve_problem_3(Graph* graph, Location source, Location dest) {

    Node* s = find_nearest_node(graph, source.latitude, source.longitude, NODE_GENERIC);
    Node* d = find_nearest_node(graph, dest.latitude, dest.longitude, NODE_GENERIC);
    
    if (!s || !d) {

        printf("Could not find nearest nodes for source/destination.\n");
        return NULL;
    }

    TimeOfDay t_start = {8, 0};
    int allowed = 0xFFFF; 
    
    PathResult* res = dijkstra(graph, s->id, d->id, OPTIMIZE_COST, allowed, t_start);

    if (!res || !res->found) {

        printf("No path found for Problem 3.\n");

        if (res) {

            free_path_result(res);
        }
             return NULL;
     }
    
           Route* route = path_to_route(graph, res, source, dest, t_start, allowed);
           free_path_result(res);
           return route;
 }


 Route* solve_problem_4(Graph* graph, Location source, Location dest, TimeOfDay start_time) {

                  Node* s = find_nearest_node(graph, source.latitude, source.longitude, NODE_GENERIC);
                  Node* d = find_nearest_node(graph, dest.latitude, dest.longitude, NODE_GENERIC);
    
    if (!s || !d) {

             printf("Could not find nearest nodes for source/destination.\n");
             return NULL;
    }

    int allowed = 0xFFFF; 
    
    PathResult* res = dijkstra(graph, s->id, d->id, OPTIMIZE_COST, allowed, start_time);

    if (!res || !res->found) {

        printf("No path found for Problem 4.\n");

        if (res) {
        
             free_path_result(res);
        }
             return NULL;
    }
    
             Route* route = path_to_route(graph, res, source, dest, start_time, allowed);
              free_path_result(res);
               return route;
 }


 Route* solve_problem_5(Graph* graph, Location source, Location dest, TimeOfDay start_time) {

    Node* s = find_nearest_node(graph, source.latitude, source.longitude, NODE_GENERIC);
    Node* d = find_nearest_node(graph, dest.latitude, dest.longitude, NODE_GENERIC);
    
    if (!s || !d) {

           printf("Could not find nearest nodes for source/destination.\n");
           return NULL;
    }

    int allowed = 0xFFFF; 
    
   
    PathResult* res = dijkstra_extended(graph, s->id, d->id, OPTIMIZE_TIME, allowed, start_time, 5);

    if (!res || !res->found) {

        printf("No path found for Problem 5.\n");

        if (res) {
        
             free_path_result(res);
        }
             return NULL;
    }
    
        Route* route = path_to_route(graph, res, source, dest, start_time, allowed);
        free_path_result(res);
        return route;
 }


 Route* solve_problem_6(Graph* graph, Location source, Location dest, 
                          TimeOfDay start_time, TimeOfDay deadline) {
    
   
    Node* s = find_nearest_node(graph, source.latitude, source.longitude, NODE_GENERIC);
    Node* d = find_nearest_node(graph, dest.latitude, dest.longitude, NODE_GENERIC);
    
    if (!s || !d) {

           printf("Could not find nearest nodes for source/destination.\n");
           return NULL;
    }

    int allowed = 0xFFFF; 
    
    
    PathResult* res = dijkstra_extended(graph, s->id, d->id, OPTIMIZE_COST, allowed, start_time, 6);

    if (res && res->found) {

        Route* route = path_to_route(graph, res, source, dest, start_time, allowed);
        free_path_result(res);
        
        if (route && route_meets_deadline(route, start_time, deadline)) {
                 return route;
        }
        
       
        if (route)
        
             free_route(route);
    } 
    else {

        if (res) 
           
             free_path_result(res);
    }
    
    
    res = dijkstra_extended(graph, s->id, d->id, OPTIMIZE_TIME, allowed, start_time, 6);

    if (res && res->found) {

             Route* route = path_to_route(graph, res, source, dest, start_time, allowed);
             free_path_result(res);
             return route;
    }
    
    if (res){
    
         free_path_result(res);
    }
         printf("No path found for Problem 6.\n");
         return NULL;
}