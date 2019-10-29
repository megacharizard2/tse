#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "tools.h"

typedef struct node{
  int key;
  int count;
  struct node *next;
}node_t;

typedef struct counters{
  node_t *head;
}counters_t;

typedef struct wrapper{
  char* word;
  counters_t* counter;
}wrapper_t;

static node_t *new_node(const int key);
void counters_delete(counters_t *ctrs);

wrapper_t* wrapper_new(char* word,counters_t* ctrs){
  wrapper_t *wrapper=malloc(sizeof(wrapper_t));
  if (wrapper != NULL){
    char* wordcopy=malloc(strlen(word)+1);
    if (wordcopy != NULL){
      strcpy(wordcopy,word);
      wrapper->word=wordcopy;
      wrapper->counter=ctrs;
    }
  }
  return wrapper;
}
char* wrapper_getword(wrapper_t* wrapper){
  if (wrapper != NULL){
    char* word=wrapper->word;
    return word;
  }
  return NULL;
}

counters_t* wrapper_getcounter(wrapper_t* wrapper){
  if (wrapper != NULL){
    counters_t* ctr=wrapper->counter;
    return ctr;
  }
  return NULL;
}

void wrapper_delete(wrapper_t* wrapper){
  if (wrapper != NULL){
    char* word=wrapper->word;
    counters_t* ctrs=wrapper->counter;
    if (word != NULL){
       free(word);
    }
    if (ctrs != NULL){
       counters_delete(ctrs);
    }
    free(wrapper);
  }
}

counters_t *counters_new(void){
  counters_t *counters=malloc(sizeof(counters_t));
  if (counters != NULL){
    counters->head=NULL;
  }
  return counters;
} 

void counters_add(counters_t *ctrs, const int key){
  
  if (key<0||ctrs==NULL){
    return;
  }
  int exists=0;
  for (node_t *node=ctrs->head;node != NULL;node=node->next){
    int nodekey=node->key;
    if (nodekey==key){
      exists=1;
    }
  }
  if (exists==0){
    node_t *newnode= new_node(key);
    if (newnode == NULL){
      return;
    }
    else{
      newnode->count=1;
      newnode->next=ctrs->head;
      ctrs->head=newnode;
    }
    return;
  }
  if (exists==1){
    node_t *existingnode;
    for (node_t *node=ctrs->head;node != NULL;node=node->next){
      if (node->key==key){
        existingnode=node;
      }
    }
    int currcount=existingnode->count;
    existingnode->count=currcount+1;
    return;
  }
  return;
}
  
static node_t *new_node(const int key){
  node_t *newnode=malloc(sizeof(node_t));
  if (newnode ==NULL){
    return NULL;
  }
  else{
    newnode->key=key;
    newnode->count=0;
    newnode->next=NULL;
  }
  return newnode;
}

int counters_get(counters_t *ctrs, const int key){
  if (ctrs == NULL){
    return 0;
  }
  for (node_t *node=ctrs->head;node!=NULL;node=node->next){
    if (node->key == key){
      int count=node->count;
      return count;
    }
  }
  return 0;
}

void counters_set(counters_t *ctrs, const int key, int count){
  if (ctrs == NULL||key<0||count<0){
    return;
  }
  for (node_t *node=ctrs->head;node!=NULL;node=node->next){
    if (node->key == key){
      node->count=count;
      return;
    }
  }
  node_t *node=new_node(key);
  if (node == NULL){
    return;
  }
  else{
    node->count=count;
    node->next=ctrs->head;
    ctrs->head=node;
  }
  return;
}

void counters_print(counters_t *ctrs, FILE *fp){
  if (ctrs==NULL){
    printf("NULL");
  }
  if (fp != NULL && ctrs != NULL){
    for (node_t *node=ctrs->head;node != NULL;node=node->next){
      int key=node->key;
      int count=node->count;
      fprintf(fp,"key:%d,count:%d\n",key,count);
    }
  }
}

void counters_iterate(counters_t *ctrs, void *arg,
                      void (*itemfunc)(void *arg, const int key, int count)){
  if (ctrs != NULL && itemfunc != NULL){
    for (node_t *node=ctrs->head;node != NULL;node=node->next){
      (*itemfunc)(arg,node->key,node->count);
    }
  }
}

void counters_delete(counters_t *ctrs){
  if (ctrs != NULL){
    for (node_t *node=ctrs->head;node != NULL;){
      if (node != NULL){
        node_t *next=node->next;
        free(node);
        node=next;
      }
    }
    free(ctrs);
  }
}

void *
assertp(void *p, const char *message)
{
  if (p == NULL) {
    fprintf(stderr, "MALLOC FAILED: %s\n", message);
    exit(EXIT_FAILURE);
  }
  return p;
}

char *freadlinep(FILE *fp) 
{
  // validate the parameter
  if (fp == NULL || feof(fp)) {
    return NULL;
  }

  // allocate buffer big enough for "typical" words/lines
  int len = 81;
  char *buf = calloc(len, sizeof(char));
  if (buf == NULL) {
    return NULL;      // out of memory
  }

  // Read characters from file until newline or EOF, 
  // expanding the buffer when needed to hold more.
  int pos;
  char c;
  for (pos = 0; (c = fgetc(fp)) != EOF && (c != '\n'); pos++) {
    // We need to save buf[pos+1] for the terminating null
    // and buf[len-1] is the last usable slot, 
    // so if pos+1 is past that slot, we need to grow the buffer.
    if (pos+1 > len-1) {
      char *newbuf = realloc(buf, ++len);
      if (newbuf == NULL) {
        free(buf);
        return NULL;      // out of memory
      } else {
        buf = newbuf;
      }
    }
    buf[pos] = c;
  }

  if (pos == 0 && c == EOF) {
    // no characters were read and we reached EOF
    free(buf);
    return NULL;
  } else {
    // pos characters were read into buf[0]..buf[pos-1].
    buf[pos] = '\0'; // terminate string
    return buf;
  }
}

/* freadlinep - see readlinep.h for description */
char *readrest(FILE *fp) 
{
  // validate the parameter
  if (fp == NULL || feof(fp)) {
    return NULL;
  }

  // allocate buffer big enough for "typical" words/lines
  int len = 81;
  char *buf = calloc(len, sizeof(char));
  if (buf == NULL) {
    return NULL;      // out of memory
  }

  // Read characters from file until newline or EOF, 
  // expanding the buffer when needed to hold more.
  int pos;
  char c;
  for (pos = 0; (c = fgetc(fp)) != EOF; pos++) {
    // We need to save buf[pos+1] for the terminating null
    // and buf[len-1] is the last usable slot, 
    // so if pos+1 is past that slot, we need to grow the buffer.
    if (pos+1 > len-1) {
      char *newbuf = realloc(buf, ++len);
      if (newbuf == NULL) {
        free(buf);
        return NULL;      // out of memory
      } else {
        buf = newbuf;
      }
    }
    buf[pos] = c;
  }

  if (pos == 0 && c == EOF) {
    // no characters were read and we reached EOF
    free(buf);
    return NULL;
  } else {
    // pos characters were read into buf[0]..buf[pos-1].
    buf[pos] = '\0'; // terminate string
    return buf;
  }
}
