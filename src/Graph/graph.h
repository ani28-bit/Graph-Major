#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"
#include "../Utils/common.h"


typedef struct Edge {
    int to_node_id;
    double distance;        
    TransportMode mode;
    struct Edge* next; 
} Edge;

typedef struct Graph {
    int num_nodes;
    int capacity;           
    Node** nodes;         
    Edge** adjacency_list; 
} Graph;


Graph* create_graph(int initial_capacity);

Node* find_node_by_location(Graph* graph, double lat, double lon, double threshold);
bool add_bidirectional_edge(Graph* graph, int from_id, int to_id, double distance, TransportMode mode);
int add_node(Graph* graph, double lat, double lon, const char* name, NodeType type);
bool add_edge(Graph* graph, int from_id, int to_id, double distance, TransportMode mode);


void free_graph(Graph* graph);
void print_graph_stats(Graph* graph);

Node* get_node(Graph* graph, int node_id);
Edge* get_neighbors(Graph* graph, int node_id);
Node* find_nearest_node(Graph* graph, double lat, double lon, NodeType type);
#endif