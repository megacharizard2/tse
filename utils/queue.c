#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <queue.h>

typedef struct queuenode{
  void* item;
  struct queuenode* next;
}queuenode_t;

typedef struct queue{
  struct queuenode* head;
  struct queuenode* tail;
} queue_s;

static queuenode_t* queuenode_new(void* item);

queue_t* qopen(void){
  queue_s* queue=(queue_s*)malloc(sizeof(queue_s));
  if (queue == NULL){
    return NULL;
  }
  else{
    queue->head=NULL;
    queue->tail=NULL;
    queue_t* qp=(queue_t*)queue;
    return qp;
  }
}

int32_t qput(queue_t *qp, void *elementp){
  if (qp != NULL && elementp != NULL){
    queue_s* queue=(queue_s*)qp;
    queuenode_t *toput= queuenode_new(elementp);
    if (toput == NULL){
      return 1;
    }
    if (queue->head == NULL){
        queue->head=toput;
        queue->tail=toput;
        return 0;
    }
    else{
      queue->tail->next=toput;
      queue->tail=toput;
      return 0;
    }
  }
  return 1;
}

void* qget(queue_t *qp){
  if (qp == NULL){
    return NULL;
  }
  queue_s* queue=(queue_s*)qp;
  if (queue->head == NULL || queue->tail == NULL){
    return NULL;
  }
  else{
    queuenode_t* first=queue->head;
    void* item=first->item;
    queue->head= queue->head->next;
    free(first);
    return item;
  }
  return NULL;
}

void qapply(queue_t *qp, void (*fn)(void* elementp)){
  if (qp != NULL && fn != NULL){
    queue_s* queue=(queue_s*)qp;
    for(queuenode_t* node=queue->head;node != NULL;node=node->next){
      fn(node->item);
    }
  }
}
void* qsearch(queue_t *qp, bool (*searchfn)(void* elementp,const void* keyp),const void* skeyp){
  if (qp != NULL && searchfn != NULL){
    queue_s* queue=(queue_s*)qp;
    for(queuenode_t* node=queue->head;node != NULL;node=node->next){
      bool itemmatch=searchfn(node->item,skeyp);
      if (itemmatch == true){
        void* item=node->item;
        return item;
      }
    }
  }
  return NULL;
}

void* qremove(queue_t *qp, bool (*searchfn)(void* elementp,const void* keyp),const void* skeyp){
  if (qp != NULL && searchfn != NULL){
    queue_s* queue=(queue_s*)qp;
    if (queue->head == NULL){
      return NULL;
    }
    bool headmatches=searchfn(queue->head->item,skeyp);
    if (headmatches == true){
      queuenode_t* currhead=queue->head;
      void* headitem=queue->head->item;
      queue->head=currhead->next;
      free(currhead);
      return headitem;
    } 
    queuenode_t* prev=queue->head;
    for(queuenode_t* node=queue->head->next;node != NULL;node=node->next){
      bool itemmatch=searchfn(node->item,skeyp);
      if (itemmatch == true){
        queuenode_t* currnode=node;
        void* item=node->item;
        prev->next=currnode->next;
        if (currnode == queue->tail){
          queue->tail=prev;
        }
        free(currnode);
        return item;
      }
      prev=node;
    }
  }
  return NULL;
}

void qclose(queue_t *qp){
  if (qp != NULL){
    queue_s* queue=(queue_s*)qp;
    for(queuenode_t* node=queue->head;node != NULL;){
      queuenode_t* next=node->next;
      free(node);
      node=next;
    }
    free(queue);
  }
}

void qconcat(queue_t *q1p, queue_t *q2p){
  if (q1p != NULL && q2p != NULL){
    queue_s* queue1=(queue_s*)q1p;
    queue_s* queue2=(queue_s*)q2p;
    if (queue2->tail != NULL && queue2->head != NULL){
      if (queue1->tail != NULL && queue1->head != NULL){
        queue1->tail->next=queue2->head;
        queue1->tail=queue2->tail;
      }
      if (queue1->tail == NULL && queue1->head ==NULL){
        queue1->head=queue2->head;
        queue1->tail=queue2->tail;
      }
    }
    free(queue2);
  }
}

static queuenode_t* queuenode_new(void* item){
  queuenode_t* node=(queuenode_t*)malloc(sizeof(queuenode_t));
  if (node == NULL){
    return NULL;
  }
  else{
    node->item = item;
    node->next= NULL;
    return node;
  }
  return NULL;
}
