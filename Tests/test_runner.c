#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include "../src/Graph/graph.h"
#include "../src/Algorithms/dijkstra.h"
#include "../src/Utils/haversine.h"


static TimeOfDay t_default = {8, 0}; 


 #define EPSILON 0.001

bool is_close(double a, double b) {

    return fabs(a - b) < EPSILON;
}

void test_haversine() {

    printf("Testing Haversine Distance...\n");
    double dist = haversine_distance(23.8103, 90.4125, 22.3569, 91.7832);
    assert(dist > 240.0 && dist < 250.0);
    assert(is_close(haversine_distance(23.8, 90.4, 23.8, 90.4), 0.0));
    printf("✓ Haversine tests passed\n\n");
}

void test_graph() {

    printf("Testing Graph Operations...\n");
    
    Graph* g = create_graph(10);
    assert(g != NULL);
    
   
    int n1 = add_node(g, 23.8, 90.4, "Node1", NODE_GENERIC);
    int n2 = add_node(g, 23.9, 90.5, "Node2", NODE_GENERIC);
    
    
    assert(g->num_nodes == 2);
    
    add_edge(g, n1, n2, 5.0, TRANSPORT_CAR);
    
    
    assert(g->adjacency_list[n1] != NULL);
    assert(g->adjacency_list[n1]->to_node_id == n2);
    
    free_graph(g);
    printf("Graph tests passed\n\n");
}



void test_dijkstra() {

    printf("Testing Dijkstra's Algorithm...\n");
    
    Graph* g = create_graph(10);
    
    int n0 = add_node(g, 23.0, 90.0, "A", NODE_GENERIC);
    int n1 = add_node(g, 23.1, 90.1, "B", NODE_GENERIC);
    int n2 = add_node(g, 23.2, 90.2, "C", NODE_GENERIC);
    
   
    add_edge(g, n0, n1, 10.0, TRANSPORT_CAR);
    add_edge(g, n1, n2, 15.0, TRANSPORT_CAR);
    
    
    add_edge(g, n0, n2, 50.0, TRANSPORT_CAR);
    
    int allowed = (1 << TRANSPORT_CAR);
    
    
    PathResult* result = dijkstra(g, n0, n2, OPTIMIZE_DISTANCE, allowed, t_default);
    
    assert(result != NULL);
    assert(result->found == true);
    assert(result->path_length == 3); 
    
    
    assert(is_close(result->total_cost, 25.0)); 
    
    assert(result->path[0] == n0);
    assert(result->path[1] == n1);
    assert(result->path[2] == n2);
    
    free_path_result(result);
    free_graph(g);
    
    printf(" Dijkstra tests passed (Path length and weight verified)\n\n");
 }



 int main() {

       printf(" Running Comprehensive Tests \n\n");
    
       test_haversine();
       test_graph();
       test_dijkstra();
    
       printf("=== All Tests Passed Successfully! ===\n");
    
       return 0;
}