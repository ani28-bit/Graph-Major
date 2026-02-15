#ifndef ASTAR_H
#define ASTAR_H

#include "../Graph/graph.h"
#include "dijkstra.h" 
#include "../Routing/time_scheduler.h"


PathResult* astar(Graph* graph, int source_id, int dest_id,
                   OptimizationCriterion criterion,
                    int allowed_modes, TimeOfDay start_time);

 double heuristic(Graph* graph, int node_id, int dest_id, OptimizationCriterion criterion);

 #endif