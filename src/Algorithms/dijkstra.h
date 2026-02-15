#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "../Graph/graph.h"
#include "../Models/route.h"
#include "../Utils/common.h"


 typedef struct PathResult {

     int* path;              
     TransportMode* modes;   
     int path_length;        
     double total_cost;      
     bool found;             
 } PathResult;


 PathResult* dijkstra(Graph* graph, int source_id, int dest_id,
                       OptimizationCriterion criterion,
                        int allowed_modes, TimeOfDay start_time);

 PathResult* dijkstra_extended(Graph* graph, int source_id, int dest_id,
                                OptimizationCriterion criterion,
                                 int allowed_modes, TimeOfDay start_time,
                                  int problem_number);

 int* reconstruct_path(int* parent, int source_id, int dest_id, int* path_length);

 void free_path_result(PathResult* result);


 void print_path(PathResult* result, Graph* graph, TimeOfDay start_time);

 #endif 