#include "linked_list.h"

LinkedList* list_create() {

    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));

    if (!list) 

        return NULL;
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    
    return list;
}

void list_append(LinkedList* list, void* data) {

    if (!list || !data) 

           return; 
    
    ListNode* node = (ListNode*)malloc(sizeof(ListNode));

      if (!node) 
      
            return;
    
    node->data = data;
    node->next = NULL;
    
    if (list->tail) {

           list->tail->next = node;
           list->tail = node;
      }
     else {

            list->head = node;
            list->tail = node;
      }
    
             list->size++;
  }


 void* list_get(LinkedList* list, int index) {

    if (!list || index < 0 || index >= list->size) {

                 return NULL;
    }
    
   
    ListNode* current = list->head;

    for (int i = 0; i < index; i++) {

             current = current->next;
     }
    
              return current->data;
  }

 void* list_remove_first(LinkedList* list) {

           if (!list || !list->head) 
                  return NULL;
    
    ListNode* to_remove = list->head;
    void* data = to_remove->data;
    
    list->head = list->head->next;

    if (!list->head) {

           list->tail = NULL;
    }
    
      free(to_remove);
      list->size--;
      return data;
}

 void list_clear(LinkedList* list) {

        if (!list) 

             return;
    
    ListNode* current = list->head;

    while (current) {

        ListNode* next = current->next;
        
        free(current);
        current = next;
    }
    
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

 void list_free(LinkedList* list) {

      if (!list) 

            return;

       list_clear(list);
       free(list);
}

 void list_foreach(LinkedList* list, void (*callback)(void* data)) {

      if (!list || !callback) 
      
               return;
    
    ListNode* current = list->head;

    while (current) {

               callback(current->data);
               current = current->next;
     }
 }