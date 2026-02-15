#ifndef ROUTE_FINDER_H
#define ROUTE_FINDER_H

#include "../Graph/graph.h"
#include "../Algorithms/dijkstra.h"
#include "../Models/route.h"

 Route* path_to_route(Graph* graph, PathResult* path_result,
                       Location source, Location dest,
                        TimeOfDay start_time, int allowed_modes);

 Route* solve_problem_1(Graph* graph, Location source, Location dest);

 Route* solve_problem_2(Graph* graph, Location source, Location dest);


 Route* solve_problem_3(Graph* graph, Location source, Location dest);

 Route* solve_problem_4(Graph* graph, Location source, Location dest,
                         TimeOfDay start_time);

 Route* solve_problem_5(Graph* graph, Location source, Location dest,
                         TimeOfDay start_time);

 Route* solve_problem_6(Graph* graph, Location source, Location dest,
                         TimeOfDay start_time, TimeOfDay deadline);

 #endif 