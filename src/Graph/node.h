#ifndef NODE_H
#define NODE_H

#include "../Utils/common.h"
#include "../Utils/linked_list.h" 

typedef struct Node {

      int id;
      double latitude;
      double longitude;
      char name[MAX_NAME_LENGTH];
      NodeType type;
      LinkedList* edges; 
 } Node;


typedef struct {

       double dist;       
       int parent;        
 } SearchState;

 Node* create_node(int id, double lat, double lon, const char* name, NodeType type);
 void free_node(Node* node);


 void node_add_edge(Node* from, int to_node_id, double distance, TransportMode mode);

 #endif