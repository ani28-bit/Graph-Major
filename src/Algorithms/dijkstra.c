#include "dijkstra.h"
#include "../Utils/priority_queue.h"
#include "../Models/transport.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


 static TimeOfDay calculate_current_time(TimeOfDay start, double minutes_passed) {

      int total_minutes = start.hour * 60 + start.minute + (int)(minutes_passed + 0.5);
      TimeOfDay new_time;
      new_time.hour = (total_minutes / 60) % 24;
      new_time.minute = total_minutes % 60;
      return new_time;
 }


 static double calculate_edge_weight(Edge* edge, OptimizationCriterion criterion, 
                                      TimeOfDay current_time, int problem_number) {

       TransportInfo info;
    
    
    if (problem_number == 5) {

              info = get_transport_info_p5(edge->mode);
    } 
    else if (problem_number == 6) {

              info = get_transport_info_p6(edge->mode);
    }
     else {
              info = get_transport_info(edge->mode);
    }
    
    switch (criterion) {

        case OPTIMIZE_DISTANCE:

                 return edge->distance;
        
        case OPTIMIZE_COST:
            
                 return edge->distance * info.cost_per_km;
        
        case OPTIMIZE_TIME: {

           
                 double travel_time = (edge->distance / info.speed_kmh) * 60.0;
            
            
            int wait = 0;

            if (edge->mode == TRANSPORT_METRO || 
                edge->mode == TRANSPORT_BUS_BIKOLPO || 
                edge->mode == TRANSPORT_BUS_UTTARA) {

                       wait = calculate_waiting_time(edge->mode, current_time);
            }
            
                      return travel_time + (double)wait;
        }
        
        default:

                      return edge->distance;
    }
 }


 PathResult* dijkstra(Graph* graph, int source_id, int dest_id,
                       OptimizationCriterion criterion,
                        int allowed_modes, TimeOfDay start_time) {

    
             return dijkstra_extended(graph, source_id, dest_id, criterion,allowed_modes, start_time, 0);
  }


 PathResult* dijkstra_extended(Graph* graph, int source_id, int dest_id,
                                OptimizationCriterion criterion,
                                 int allowed_modes, TimeOfDay start_time,
                                  int problem_number) {
    
     if (!graph || source_id < 0 || dest_id < 0 || 
           source_id >= graph->num_nodes || dest_id >= graph->num_nodes) {

              return NULL;
    }
    
    int node_count = graph->num_nodes;
    
   
    double* dist = (double*)malloc(node_count * sizeof(double));
    int* parent = (int*)malloc(node_count * sizeof(int));
    bool* visited = (bool*)calloc(node_count, sizeof(bool));
    double* time_taken = (double*)malloc(node_count * sizeof(double));
    
    if (!dist || !parent || !visited || !time_taken) {

          if (dist) 

               free(dist);

          if (parent) 

              free(parent);

          if (visited)

              free(visited);

          if (time_taken)

              free(time_taken);

               return NULL;
    }
    
    
    for (int i = 0; i < node_count; i++) {

        dist[i] = INFINITY;
        time_taken[i] = 0.0;
        parent[i] = -1;
    }
    
    dist[source_id] = 0.0;
    time_taken[source_id] = 0.0;
    
    PriorityQueue* pq = pq_create(node_count);

     if (!pq) {

           free(dist); free(parent); free(visited); free(time_taken);
           return NULL;
    }
    
    pq_push(pq, source_id, 0.0);

    
    while (!pq_is_empty(pq)) {

        PQNode current = pq_pop(pq);
        int u = current.node_id;
        
        if (visited[u]) {
              continue;
        }

              visited[u] = true;
        
        
        if (u == dest_id) {

                break;
        }

       
        TimeOfDay current_u_time = calculate_current_time(start_time, time_taken[u]);

        
        Edge* edge = get_neighbors(graph, u);

        while (edge) {

             int v = edge->to_node_id;
            
            
            
             if (!(allowed_modes & (1 << edge->mode))) {

                   edge = edge->next;
                   continue;
         }

            
            
            if (!is_transport_available(edge->mode, current_u_time)) {

                     edge = edge->next;
                     continue;
         }
            
          
            double weight = calculate_edge_weight(edge, criterion, current_u_time, problem_number);
            
            
            TransportInfo info;

            if (problem_number == 5) {

                           info = get_transport_info_p5(edge->mode);
            } 
            else if (problem_number == 6) {

                           info = get_transport_info_p6(edge->mode);
            } 
            else {
                          info = get_transport_info(edge->mode);
             }
            
            int wait = calculate_waiting_time(edge->mode, current_u_time);
            double actual_travel_mins = (edge->distance / info.speed_kmh) * 60.0 + (double)wait;

           
            double alt = dist[u] + weight;
            
            if (alt < dist[v]) {

                   dist[v] = alt;
                   parent[v] = u;
                   time_taken[v] = time_taken[u] + actual_travel_mins;
                   pq_push(pq, v, alt);
            }
            
                   edge = edge->next;
        }
    }
    
   
    PathResult* result = (PathResult*)malloc(sizeof(PathResult));
    if (!result) {

         free(dist); free(parent); free(visited); free(time_taken);
         pq_free(pq);
         return NULL;
    }
    
    result->found = (dist[dest_id] != INFINITY);
    result->modes = NULL; 
    
    if (result->found) {

          result->total_cost = dist[dest_id];
          result->path = reconstruct_path(parent, source_id, dest_id, &result->path_length);
    } 
    else {
           result->path = NULL;
           result->path_length = 0;
           result->total_cost = 0.0;
    }
    
   
      free(dist);
      free(parent);
      free(visited);
      free(time_taken);
      pq_free(pq);
    
      return result;
 }


  int* reconstruct_path(int* parent, int source_id, int dest_id, int* path_length) {

          if (!parent || !path_length) {
                 return NULL;
          }
    
    
    int count = 0;
    int current = dest_id;
    
    while (current != -1 && count < 100000) { 
              count++;

        if (current == source_id) {
              break;
        }
              current = parent[current];
    }
    
   
    if (count == 0 || (current != source_id && source_id != dest_id)) {

               *path_length = 0;
                return NULL;
    }
    
   
     int* path = (int*)malloc(count * sizeof(int));
    if (!path) {

         *path_length = 0;
          return NULL;
    }
    
    
    current = dest_id;

    for (int i = count - 1; i >= 0; i--) {

        path[i] = current;
        if (current == source_id) {
               break;
        }
              current = parent[current];
    }
    
     *path_length = count;
      return path;
 }


 void free_path_result(PathResult* result) {

    if (result) {

        if (result->path){

              free(result->path);
        }

        if (result->modes){
              free(result->modes);
        }
              free(result);
     }
 }


 void print_path(PathResult* result, Graph* graph, TimeOfDay start_time) {

     if (!result || !result->found || !result->path || !graph) {

               printf("No path found.\n");
               return;
    }
    
      printf("\n=== Path Details ===\n");
      printf("Total nodes: %d\n", result->path_length);
      printf("Total cost/distance/time: %.2f\n", result->total_cost);
      printf("Start time: %02d:%02d\n\n", start_time.hour, start_time.minute);
    
    TimeOfDay current_time = start_time;
    
    for (int i = 0; i < result->path_length; i++) {

           Node* node = get_node(graph, result->path[i]);

        if (node) {

                printf("Node %d: %s (%.6f, %.6f) at %02d:%02d\n",
                        result->path[i], 
                        node->name[0] ? node->name : "Unnamed",
                        node->latitude, node->longitude,
                        current_time.hour, current_time.minute);
        }
        
       
        if (i < result->path_length - 1) {

             Edge* edge = get_neighbors(graph, result->path[i]);
            while (edge) {

                   if (edge->to_node_id == result->path[i + 1]) {

                            TransportInfo info = get_transport_info(edge->mode);
                            int wait = calculate_waiting_time(edge->mode, current_time);
                            double travel = (edge->distance / info.speed_kmh) * 60.0;
                    
                        int total_mins = (int)(wait + travel);
                        current_time.minute += total_mins;
                        current_time.hour += current_time.minute / 60;
                        current_time.minute %= 60;
                        current_time.hour %= 24;
                    
                        printf("  -> Mode: %s, Distance: %.2f km, Time: %d mins\n",
                                 info.name, edge->distance, total_mins);

                                 break;
                }
                                 edge = edge->next;
            }
        }
    }
    
      printf("\n\n");
}