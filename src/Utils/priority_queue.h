#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "common.h"


typedef struct PQNode {

     int node_id;
     double priority;  
 } PQNode;

 typedef struct PriorityQueue {

       PQNode* nodes;
       int* positions;    
       int size;
       int capacity;     
  } PriorityQueue;

 PriorityQueue* pq_create(int capacity);

 bool pq_is_empty(PriorityQueue* pq);

 void pq_push(PriorityQueue* pq, int node_id, double priority);

 PQNode pq_pop(PriorityQueue* pq);

 void pq_decrease_priority(PriorityQueue* pq, int node_id, double new_priority);

 bool pq_contains(PriorityQueue* pq, int node_id);

 void pq_free(PriorityQueue* pq);

 #endif