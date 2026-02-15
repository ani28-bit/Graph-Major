#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "../src/Graph/graph.h"
#include "../src/Algorithms/dijkstra.h"
#include "../src/Models/transport.h" 


 static TimeOfDay t_start = {8, 0}; 

 void test_simple_path() {

    printf("Testing simple path...\n");
    Graph* graph = create_graph(10);
    
    int a = add_node(graph, 0, 0, "A", NODE_GENERIC);
    int b = add_node(graph, 0, 1, "B", NODE_GENERIC);
    int c = add_node(graph, 0, 2, "C", NODE_GENERIC);
    
    add_edge(graph, a, b, 10.0, TRANSPORT_CAR);
    add_edge(graph, b, c, 15.0, TRANSPORT_CAR);
    
    int allowed = (1 << TRANSPORT_CAR);
    
    PathResult* result = dijkstra(graph, a, c, OPTIMIZE_DISTANCE, allowed, t_start);
    
    assert(result != NULL);
    assert(result->found == true);
    assert(result->path_length == 3);
    assert(result->total_cost == 25.0);
    
    free_path_result(result);
    free_graph(graph);
    printf("simple path passed\n");
 }

 void test_shortest_of_multiple_paths() {

    printf("Testing shortest of multiple paths.\n");
    Graph* graph = create_graph(10);
    
    int a = add_node(graph, 0, 0, "A", NODE_GENERIC);
    int b = add_node(graph, 0, 1, "B", NODE_GENERIC);
    int c = add_node(graph, 1, 1, "C", NODE_GENERIC);
    int d = add_node(graph, 1, 2, "D", NODE_GENERIC);
    
    add_edge(graph, a, b, 10.0, TRANSPORT_CAR);
    add_edge(graph, b, d, 10.0, TRANSPORT_CAR);
    add_edge(graph, a, c, 5.0, TRANSPORT_CAR);
    add_edge(graph, c, d, 5.0, TRANSPORT_CAR);
    
    int allowed = (1 << TRANSPORT_CAR);
    
    PathResult* result = dijkstra(graph, a, d, OPTIMIZE_DISTANCE, allowed, t_start);
    
    assert(result != NULL);
    assert(result->found == true);
    assert(result->total_cost == 10.0); 
    
    free_path_result(result);
    free_graph(graph);
    printf("shortest of multiple paths passed\n");
 }

 void test_cost_optimization() {

    printf("Testing cost optimization.\n");
    Graph* graph = create_graph(10);
    
    int a = add_node(graph, 0, 0, "A", NODE_GENERIC);
    int b = add_node(graph, 0, 1, "B", NODE_GENERIC);
    
    
    add_edge(graph, a, b, 10.0, TRANSPORT_CAR);   
    add_edge(graph, a, b, 12.0, TRANSPORT_METRO); 
    
    int allowed = (1 << TRANSPORT_CAR) | (1 << TRANSPORT_METRO);
   
    PathResult* result = dijkstra(graph, a, b, OPTIMIZE_COST, allowed, t_start);
    
    assert(result != NULL);
    assert(result->found == true);
   
    assert(result->total_cost < 200.0); 
    
    free_path_result(result);
    free_graph(graph);
    printf("cost optimization passed\n");
}

 void test_no_path() {

    printf("Testing no path exists.\n");

    Graph* graph = create_graph(10);
    int a = add_node(graph, 0, 0, "A", NODE_GENERIC);
    int b = add_node(graph, 0, 1, "B", NODE_GENERIC);
    int c = add_node(graph, 1, 1, "C", NODE_GENERIC);
    add_edge(graph, a, b, 10.0, TRANSPORT_CAR);
    
    int allowed = (1 << TRANSPORT_CAR);
    
    PathResult* result = dijkstra(graph, a, c, OPTIMIZE_DISTANCE, allowed, t_start);
    
    assert(result != NULL);
    assert(result->found == false);
    
    free_path_result(result);
    free_graph(graph);
    printf("✓ no path passed\n");
 }

 void test_mode_restriction() {

    printf("Testing transport mode restriction.\n");

    Graph* graph = create_graph(10);
    int a = add_node(graph, 0, 0, "A", NODE_GENERIC);
    int b = add_node(graph, 0, 1, "B", NODE_GENERIC);
    int c = add_node(graph, 1, 1, "C", NODE_GENERIC);
    
    add_edge(graph, a, b, 10.0, TRANSPORT_CAR);
    add_edge(graph, b, c, 10.0, TRANSPORT_METRO);
    
    int allowed = (1 << TRANSPORT_CAR);
   
    PathResult* result = dijkstra(graph, a, c, OPTIMIZE_DISTANCE, allowed, t_start);
    assert(result->found == false);
    free_path_result(result);
    
    allowed = (1 << TRANSPORT_CAR) | (1 << TRANSPORT_METRO);
    result = dijkstra(graph, a, c, OPTIMIZE_DISTANCE, allowed, t_start);
    assert(result->found == true);
    
    free_path_result(result);
    free_graph(graph);
    printf("mode restriction passed\n");
}

 void test_same_source_dest() {

    printf("Testing same source and destination.\n");
    Graph* graph = create_graph(10);
    int a = add_node(graph, 0, 0, "A", NODE_GENERIC);
    
    int allowed = (1 << TRANSPORT_CAR);
    
    PathResult* result = dijkstra(graph, a, a, OPTIMIZE_DISTANCE, allowed, t_start);
    
    assert(result != NULL);
    assert(result->found == true);
    assert(result->path_length == 1);
    assert(result->total_cost == 0.0);
    
    free_path_result(result);
    free_graph(graph);
    printf("same source/dest passed\n");
 }

 int main() {

    printf(" Running Dijkstra Tests \n\n");
    
    test_simple_path();
    test_shortest_of_multiple_paths();
    test_no_path();
    test_cost_optimization();
    test_mode_restriction();
    test_same_source_dest();
    
    printf("\n All Dijkstra Tests Passed! \n");
    return 0;
}