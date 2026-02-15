#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "../src/Graph/graph.h"
#include "../src/Utils/haversine.h"


#define EPSILON 0.00001

bool double_eq(double a, double b) {

          return (a > b ? a - b : b - a) < EPSILON;
} 

 void test_create_graph() {

    printf("Testing create_graph...\n");
    Graph* graph = create_graph(100);

    assert(graph != NULL);
    assert(graph->num_nodes == 0);
    assert(graph->capacity == 100);
    free_graph(graph);

    printf("create_graph passed\n");
 }

 void test_add_node() {

    printf("Testing add_node...\n");
    Graph* graph = create_graph(10);
    
    int id1 = add_node(graph, 23.8103, 90.4125, "Shahbagh", NODE_ROAD);

    assert(id1 == 0);
    assert(graph->num_nodes == 1);
    
    Node* node1 = get_node(graph, id1);

    assert(node1 != NULL);
    assert(double_eq(node1->latitude, 23.8103));
    assert(double_eq(node1->longitude, 90.4125));
    
    free_graph(graph);
    printf("add_node passed\n");
 }

 void test_add_edge() {

    printf("Testing add_edge.\n");
    Graph* graph = create_graph(10);

    int n1 = add_node(graph, 23.8, 90.4, "A", NODE_GENERIC);
    int n2 = add_node(graph, 23.9, 90.5, "B", NODE_GENERIC);
    
    
    bool result = add_edge(graph, n1, n2, 10.5, TRANSPORT_CAR);
    assert(result == true);
    assert(get_num_neighbors(graph, n1) == 1);
    assert(get_num_neighbors(graph, n2) == 0); 
    
    free_graph(graph);
    printf("add_edge passed\n");
 }

 void test_find_nearest_node() {

    printf("Testing find_nearest_node...\n");
    Graph* graph = create_graph(10);
    
    add_node(graph, 23.8, 90.4, "A", NODE_GENERIC);
    add_node(graph, 23.9, 90.5, "B", NODE_GENERIC);
    int c_id = add_node(graph, 23.85, 90.45, "C", NODE_GENERIC);
    
   
    Node* nearest = find_nearest_node(graph, 23.851, 90.451, NODE_GENERIC);
    assert(nearest != NULL);
    assert(nearest->id == c_id);
    
    free_graph(graph);
    printf("✓ find_nearest_node passed\n");
}

 void test_graph_resize() {

    printf("Testing graph resize.\n");
    Graph* graph = create_graph(2); 
    
    for (int i = 0; i < 10; i++) {

                  add_node(graph, 23.8 + i, 90.4 + i, "Node", NODE_GENERIC);
    }

    assert(graph->num_nodes == 10);
    assert(graph->capacity >= 10);
    
    free_graph(graph);
    printf("graph resize passed\n");
}

 int main() {

    printf(" Running Graph Tests\n\n");
    test_create_graph();
    test_add_node();
    test_add_edge();
    test_bidirectional_edge(); 
    test_find_nearest_node();
    test_graph_resize();
    test_multiple_edges();
    
    printf("\nAll Graph Tests Passed!\n");
    return 0;
}