#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "graph.h"
#include "../Utils/haversine.h"


 Graph* create_graph(int initial_capacity) {

       Graph* graph = (Graph*)malloc(sizeof(Graph));

        if (!graph)
             return NULL;
    
    graph->num_nodes = 0;
    graph->capacity = initial_capacity;
    
    graph->nodes = (Node**)calloc(initial_capacity, sizeof(Node*));
    graph->adjacency_list = (Edge**)calloc(initial_capacity, sizeof(Edge*));
    
    if (!graph->nodes || !graph->adjacency_list) {

        if (graph->nodes) 
                free(graph->nodes);

        if (graph->adjacency_list) 
                free(graph->adjacency_list);
                 free(graph);
                  return NULL;
     }
    
                   return graph;
 }


 static void resize_graph(Graph* graph) {

      int new_capacity = graph->capacity * 2;
    
      Node** new_nodes = (Node**)realloc(graph->nodes, new_capacity * sizeof(Node*));
      Edge** new_adj_list = (Edge**)realloc(graph->adjacency_list, new_capacity * sizeof(Edge*));
    
      if (!new_nodes || !new_adj_list) {

              fprintf(stderr, "Memory allocation failed during graph resize.\n");
              exit(1);
    }
    
    
    for (int i = graph->capacity; i < new_capacity; i++) {

              new_nodes[i] = NULL;
              new_adj_list[i] = NULL;
    }
    
             graph->nodes = new_nodes;
             graph->adjacency_list = new_adj_list;
             graph->capacity = new_capacity;
 }

 int add_node(Graph* graph, double lat, double lon, const char* name, NodeType type) {

             if (!graph) 
                   return -1;
    
             if (graph->num_nodes >= graph->capacity) {

                   resize_graph(graph);
    }
    
            int node_id = graph->num_nodes;
            graph->nodes[node_id] = create_node(node_id, lat, lon, name, type);
            graph->adjacency_list[node_id] = NULL;
            graph->num_nodes++;
    
            return node_id;
 }

 bool add_edge(Graph* graph, int from_id, int to_id, double distance, TransportMode mode) {

         if (!graph || from_id < 0 || from_id >= graph->num_nodes || 
              to_id < 0 || to_id >= graph->num_nodes) {

                        return false;
    }

     Edge* new_edge = (Edge*)malloc(sizeof(Edge));
        if (!new_edge)
              return false;
    
     new_edge->to_node_id = to_id;
     new_edge->distance = distance;
     new_edge->mode = mode;
    
   
     new_edge->next = graph->adjacency_list[from_id];
     graph->adjacency_list[from_id] = new_edge;
    
     return true;
 }


 bool add_bidirectional_edge(Graph* graph, int from_id, int to_id,double distance, TransportMode mode) {

             if (!graph) 
                   return false;
    
       bool forward = add_edge(graph, from_id, to_id, distance, mode);
       bool backward = add_edge(graph, to_id, from_id, distance, mode);
    
       return forward && backward;
 }


 Node* find_node_by_location(Graph* graph, double lat, double lon, double threshold) {

            if (!graph) 
                return NULL;
    
    for (int i = 0; i < graph->num_nodes; i++) {

        Node* node = graph->nodes[i];

        if (!node)
             continue;
        
        double dist = haversine_distance(lat, lon, node->latitude, node->longitude);

        if (dist < threshold) {

               return node;
          }
     }
    
            return NULL;
 }


  void connect_transport_to_roads(Graph* graph) {

              if (!graph) 
                   return;

         printf("Connecting Metro and Bus hubs to Road network...\n");
         int connections = 0;

    for (int i = 0; i < graph->num_nodes; i++) {

        Node* hub = graph->nodes[i];
        
       
        if (hub->type == NODE_METRO_STATION || hub->type == NODE_BUS_STOP) {

            Node* nearest_road = NULL;
            double min_dist = 0.5; 
            
            for (int j = 0; j < graph->num_nodes; j++) {
                Node* road = graph->nodes[j];

                if (road->type == NODE_ROAD) {

                    double d = haversine_distance(hub->latitude, hub->longitude, 
                                                 road->latitude, road->longitude);
                    if (d < min_dist) {

                         min_dist = d;
                         nearest_road = road;
                    }
                }
            }

            if (nearest_road) {

                
                     add_bidirectional_edge(graph, hub->id, nearest_road->id, min_dist, TRANSPORT_WALK);
                     connections++;
              }
           }
       }

                     printf("Created %d walking transfer edges.\n", connections);
   }


     Node* get_node(Graph* graph, int node_id) {

       if (!graph || node_id < 0 || node_id >= graph->num_nodes) {

               return NULL;
        }
               return graph->nodes[node_id];
    }


  Edge* get_neighbors(Graph* graph, int node_id) {

         if (!graph || node_id < 0 || node_id >= graph->num_nodes) {

                    return NULL;
         }

                   return graph->adjacency_list[node_id];
    }


  Node* find_nearest_node(Graph* graph, double lat, double lon, NodeType type) {


               if (!graph || graph->num_nodes == 0) 
                       return NULL;
    
       Node* nearest = NULL;
       double min_distance = INFINITY;
    
    for (int i = 0; i < graph->num_nodes; i++) {

               Node* node = graph->nodes[i];

               if (!node) 
                    continue;
        
        
             if (type != NODE_GENERIC && node->type != type) 
                    continue;
        
            double dist = haversine_distance(lat, lon, node->latitude, node->longitude);

                   if (dist < min_distance) {

                            min_distance = dist;
                            nearest = node;
                   }
               }
                            return nearest;
         }


   void print_graph_stats(Graph* graph) {

          if (!graph) {

                 printf("Graph is NULL\n");
                 return;
    }
    
          int road_nodes = 0, metro_nodes = 0, bus_nodes = 0;
          int total_edges = 0;
    
    for (int i = 0; i < graph->num_nodes; i++) {

        Node* node = graph->nodes[i];

         if (!node) 
              continue;
        
        switch (node->type) {

            case NODE_ROAD: road_nodes++; break;
            case NODE_METRO_STATION: metro_nodes++; break;
            case NODE_BUS_STOP: bus_nodes++; break;
            default: break;
        }
        
       
        Edge* edge = graph->adjacency_list[i];

        while (edge) {

            total_edges++;
            edge = edge->next;
        }
    }
    
    printf("\nGraph Statistics : \n");
    printf("Total Nodes: %d\n", graph->num_nodes);
    printf("  - Road Intersections: %d\n", road_nodes);
    printf("  - Metro Stations: %d\n", metro_nodes);
    printf("  - Bus Stops: %d\n", bus_nodes);
    printf("Total Edges: %d\n", total_edges);
    printf("Graph Capacity: %d\n", graph->capacity);
    printf("\n\n");
 }


 void free_graph(Graph* graph) {

        if (!graph) 
             return;
    
    for (int i = 0; i < graph->num_nodes; i++) {
        
             Edge* edge = graph->adjacency_list[i];

        while (edge) {

                  Edge* next = edge->next;
                  free(edge);
                  edge = next;
        }
        
        
                free_node(graph->nodes[i]);
    }
    
         free(graph->nodes);
         free(graph->adjacency_list);
         free(graph);
}