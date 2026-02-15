#include "astar.h"
#include "../Utils/priority_queue.h"
#include "../Utils/haversine.h"
#include "../Models/transport.h"
#include "../Routing/time_scheduler.h"

#include <math.h>
#include <stdlib.h>
#include <stdbool.h>


#define HEURISTIC_MAX_SPEED 100.0   
#define INF 1e15

static double calculate_edge_weight_astar(Edge* edge, OptimizationCriterion criterion, TimeOfDay current_time) {
    TransportInfo info = get_transport_info(edge->mode);
    
    switch (criterion) {

        case OPTIMIZE_DISTANCE:

                   return edge->distance;
        
        case OPTIMIZE_COST:

                   return edge->distance * info.cost_per_km;
        
        case OPTIMIZE_TIME: {

                    double travel_time = (edge->distance / info.speed_kmh) * 60.0;
                    int wait = calculate_waiting_time(edge->mode, current_time);
                    return travel_time + (double)wait;
        }
        
        default:

                    return edge->distance;
    }
}
double heuristic(Graph* graph, int node_id, int dest_id, OptimizationCriterion criterion) {

    Node* node = get_node(graph, node_id);
    Node* dest = get_node(graph, dest_id);

     if (!node || !dest) 
              return 0.0;

    double dist = haversine_distance(node->latitude, node->longitude,
                                    dest->latitude, dest->longitude);

    switch (criterion) {

        case OPTIMIZE_TIME:
           
        
                  return (dist / HEURISTIC_MAX_SPEED) * 60.0;

        case OPTIMIZE_COST:

                 return dist * 0.0; 

        default:

                 return dist; 
    }
}

PathResult* astar(Graph* graph, int source_id, int dest_id,
                 OptimizationCriterion criterion,
                 int allowed_modes, TimeOfDay start_time) {
    
    if (!graph || source_id < 0 || dest_id < 0 || source_id >= graph->num_nodes)

               return NULL;

    int node_count = graph->num_nodes;
    double* g_score = malloc(node_count * sizeof(double));
    int* parent = malloc(node_count * sizeof(int));
    bool* visited = calloc(node_count, sizeof(bool));

    if (!g_score || !parent || !visited) {

              free(g_score); free(parent); free(visited);
              return NULL;
    }

    for (int i = 0; i < node_count; i++) {

              g_score[i] = INF;
              parent[i] = -1;
    }

      g_score[source_id] = 0.0;
      PriorityQueue* pq = pq_create(node_count);
    
    
    pq_push(pq, source_id, 0.0 + heuristic(graph, source_id, dest_id, criterion));

    while (!pq_is_empty(pq)) {

            PQNode pq_node = pq_pop(pq);
            int u = pq_node.node_id;

         if (visited[u])
                continue;

           visited[u] = true;

           if (u == dest_id) 
                 break;

      
        TimeOfDay current_u_time = start_time;
        if (criterion == OPTIMIZE_TIME) {

                    current_u_time = add_minutes_to_time(start_time, (int)(g_score[u] + 0.5));
        }

        Edge* edge = get_neighbors(graph, u);
        while (edge) {
            int v = edge->to_node_id;

            
            if (!(allowed_modes & (1 << edge->mode))) {

                       edge = edge->next; continue;
            }

          
            if (!is_transport_available(edge->mode, current_u_time)) {

                       edge = edge->next; continue;
            }

            double weight = calculate_edge_weight_astar(edge, criterion, current_u_time);
            double tentative_g = g_score[u] + weight;

            if (tentative_g < g_score[v]) {

                 parent[v] = u;
                 g_score[v] = tentative_g;
                 double h_score = heuristic(graph, v, dest_id, criterion);
                
                
                 pq_push(pq, v, tentative_g + h_score);
            }
                 edge = edge->next;
        }
    }

    PathResult* result = malloc(sizeof(PathResult));
    result->found = (g_score[dest_id] < INF);
    result->total_cost = result->found ? g_score[dest_id] : 0.0;
    result->path_length = 0;
    result->path = NULL;

    if (result->found) {

             result->path = reconstruct_path(parent, source_id, dest_id, &result->path_length);
    }

     free(g_score); free(parent); free(visited);
     pq_free(pq);
     return result;
}