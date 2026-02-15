#include "node.h"
#include "../Utils/linked_list.h" 

Node* create_node(int id, double lat, double lon, const char* name, NodeType type) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) {

        fprintf(stderr, " Memory allocation failed for node\n");
        return NULL;
    }
    
    node->id = id;
    node->latitude = lat;
    node->longitude = lon;
    node->type = type;
    
   
    node->edges = list_create(); 
    
    if (name) {

          strncpy(node->name, name, MAX_NAME_LENGTH - 1);
          node->name[MAX_NAME_LENGTH - 1] = '\0';
    } 
    else {
           node->name[0] = '\0';
    }
    
          return node;
  }

 void free_node(Node* node) {

    if (node) {

        if (node->edges) {
            
              list_free(node->edges); 
        }
              free(node);
    }
}


void node_add_edge(Node* from, int to_node_id, double distance, TransportMode mode) {

    if (!from || !from->edges) 
             return;

    
    typedef struct {
           int to_node_id;
           double weight;
           TransportMode mode;
    } Edge;

     Edge* new_edge = (Edge*)malloc(sizeof(Edge));
     new_edge->to_node_id = to_node_id;
     new_edge->weight = distance;
     new_edge->mode = mode;

     list_append(from->edges, new_edge);
}

   void print_node(Node* node) {

    if (!node) {

         printf("NULL node\n");
         return;
    }
    
         printf("Node %d: (%.6f, %.6f)", node->id, node->latitude, node->longitude);

    if (node->name[0] != '\0') {

            printf(" - %s", node->name);
    }
    
   
             printf(" [%d edges]\n", node->edges ? node->edges->size : 0);
}