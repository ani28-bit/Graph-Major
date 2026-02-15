#include "priority_queue.h"

PriorityQueue* pq_create(int capacity) {

    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));

       if (!pq) 

           return NULL;
    
    pq->nodes = (PQNode*)malloc(capacity * sizeof(PQNode));
    pq->positions = (int*)malloc(capacity * sizeof(int));
    pq->capacity = capacity;
    pq->size = 0;
    
    if (!pq->nodes || !pq->positions) {

        if (pq->nodes) 
        
                free(pq->nodes);

        if (pq->positions)

                 free(pq->positions);

              free(pq);
              return NULL;
    }
    
    for (int i = 0; i < capacity; i++) {

              pq->positions[i] = -1;
      }
    
                return pq;
  }

 bool pq_is_empty(PriorityQueue* pq) {

             return pq == NULL || pq->size == 0;
}

static void swap_pq_nodes(PriorityQueue* pq, int idx1, int idx2) {

    PQNode temp = pq->nodes[idx1];
    pq->nodes[idx1] = pq->nodes[idx2];
    pq->nodes[idx2] = temp;
    
    
    pq->positions[pq->nodes[idx1].node_id] = idx1;
    pq->positions[pq->nodes[idx2].node_id] = idx2;
}

static void heapify_up(PriorityQueue* pq, int idx) {

    while (idx > 0) {

        int parent = (idx - 1) / 2;
        if (pq->nodes[idx].priority >= pq->nodes[parent].priority) 

                break;
        swap_pq_nodes(pq, idx, parent);
        idx = parent;
    }
}

static void heapify_down(PriorityQueue* pq, int idx) {

    while (true) {

        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int smallest = idx;
        
        if (left < pq->size && pq->nodes[left].priority < pq->nodes[smallest].priority)

                   smallest = left;

        if (right < pq->size && pq->nodes[right].priority < pq->nodes[smallest].priority)

                      smallest = right;
        
        if (smallest == idx)

                       break;

               swap_pq_nodes(pq, idx, smallest);
                idx = smallest;
    }
}

void pq_push(PriorityQueue* pq, int node_id, double priority) {
   
    if (!pq || node_id < 0 || node_id >= pq->capacity || pq->size >= pq->capacity) {
                return; 
      }
    
    
    if (pq->positions[node_id] != -1) {

              pq_decrease_priority(pq, node_id, priority);
              return;
    }

         int idx = pq->size;
         pq->nodes[idx].node_id = node_id;
         pq->nodes[idx].priority = priority;
         pq->positions[node_id] = idx;
         pq->size++;
    
         heapify_up(pq, idx);
}

 PQNode pq_pop(PriorityQueue* pq) {

         PQNode min = pq->nodes[0];
         pq->positions[min.node_id] = -1; 
    
         pq->size--;

    if (pq->size > 0) {

           pq->nodes[0] = pq->nodes[pq->size];
           pq->positions[pq->nodes[0].node_id] = 0;
           heapify_down(pq, 0);
     }
    
           return min;
  }

 void pq_decrease_priority(PriorityQueue* pq, int node_id, double new_priority) {

   
    if (!pq || node_id < 0 || node_id >= pq->capacity) 
                return;
    
    int idx = pq->positions[node_id];

    if (idx == -1) {

           pq_push(pq, node_id, new_priority);
           return;
    }
    
    if (new_priority < pq->nodes[idx].priority) {

               pq->nodes[idx].priority = new_priority;
               heapify_up(pq, idx);
     }
 }

 bool pq_contains(PriorityQueue* pq, int node_id) {

      if (!pq || node_id < 0 || node_id >= pq->capacity) {

                     return false;
      }
                     return pq->positions[node_id] != -1;
  }

   void pq_free(PriorityQueue* pq) {

    if (pq) {

          free(pq->nodes);
          free(pq->positions);
          free(pq);
    }
}