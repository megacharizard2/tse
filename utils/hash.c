/* 
 * hash.c -- implements a generic hash table as an indexed set of queues.
 *
 */
#include <stdint.h>
#include "string.h"
#include <stdlib.h>
#include <hash.h>
#include <queue.h>

typedef struct hashtable{
  queue_t **hashtable;
} hashtable_s;

static uint32_t SuperFastHash(const char *data,int len,uint32_t tablesize);

hashtable_t *hopen(uint32_t hsize){
  hashtable_s* hash=(hashtable_s*)malloc(sizeof(hashtable_s));
  if (hash == NULL){
    return NULL;
  }
  else{
    hash->hashtable=calloc(hsize,sizeof(queue_t*));
  }
  hashtable_t* hashtabl=(hashtable_t*)hash;
  return hashtabl;
}

int32_t hput(hashtable_t *htp, void *ep, const char *key, int keylen){
  if (htp == NULL || ep == NULL || key == NULL){
    return 1;
  }
  hashtable_s* ht=(hashtable_s*)htp;
  uint32_t index=SuperFastHash(key,keylen,sizeof(ht));
  if (ht->hashtable[index]==NULL){
    queue_t* queue=qopen();
    if (queue == NULL){
      return 1;
    }
    ht->hashtable[index]=queue;
    int32_t insert=qput(queue,ep);
    if (insert == 0){
      return 0;
    }
    else{
      return 1;
    }
  }
  else{
    int32_t ins=qput(ht->hashtable[index],ep);
    if (ins == 0){
      return 0;
    }
    else{
      return 1;
    }
  }
  return 1;
}

void happly(hashtable_t *htp, void (*fn)(void* ep)){
  if (htp != NULL && fn != NULL){
    int size=sizeof(htp);
    hashtable_s* ht=(hashtable_s*)htp;
    for (int x=0;x<size;x++){
      if (ht->hashtable[x] != NULL){
        queue_t* queue=ht->hashtable[x];
        qapply(queue,fn);
      }
    }
  }

}
void *hsearch(hashtable_t *htp, 
              bool (*searchfn)(void* elementp, const void* searchkeyp), 
              const char *key, 
              int32_t keylen){

  if (htp == NULL || key == NULL){
    return NULL;
  }
  hashtable_s* ht=(hashtable_s*)htp;
  uint32_t index=SuperFastHash(key,keylen,sizeof(ht));
  queue_t* queue=ht->hashtable[index];
  if (queue == NULL){
    return NULL;
  }
  void* item=qsearch(queue,searchfn,key);
  if (item == NULL){
    return NULL;
  }
  return item;
}

void *hremove(hashtable_t *htp, 
              bool (*searchfn)(void* elementp, const void* searchkeyp), 
              const char *key, 
              int32_t keylen){

  if (htp == NULL || key == NULL){
    return NULL;
  }
  hashtable_s* ht=(hashtable_s*)htp;
  uint32_t index=SuperFastHash(key,keylen,sizeof(ht));
  queue_t* queue=ht->hashtable[index];
  if (queue == NULL){
    return NULL;
  }
  void* item=qremove(queue,searchfn,key);
  if (item == NULL){
    return NULL;
  }
  return item;
}
void hclose(hashtable_t *htp){
  if (htp != NULL){
    hashtable_s* ht=(hashtable_s*)htp;
    int size=sizeof(htp);
    for (int x=0;x<size;x++){
      if (ht->hashtable[x] != NULL){
        queue_t* queue=ht->hashtable[x];
        qclose(queue);
      }
    }
    free(ht->hashtable);
    free(ht);
  }
}






/*SuperFastHash() -- produces a number between 0 and the tablesize-1.

The following (rather complicated) code, has been taken from Paul
Hsieh's website under the terms of the BSD license. It's a hash
function used all over the place nowadays, including Google Sparse
Hash.*/
#define get16bits(d) (*((const uint16_t *) (d)))
static uint32_t SuperFastHash(const char *data,int len,uint32_t tablesize) {
  uint32_t hash = len, tmp;
  int rem;
  
  if (len <= 0 || data == NULL)
		return 0;
  rem = len & 3;
  len >>= 2;
  /* Main loop */
  for (;len > 0; len--) {
    hash  += get16bits (data);
    tmp    = (get16bits (data+2) << 11) ^ hash;
    hash   = (hash << 16) ^ tmp;
    data  += 2*sizeof (uint16_t);
    hash  += hash >> 11;
  }
  /* Handle end cases */
  switch (rem) {
  case 3: hash += get16bits (data);
    hash ^= hash << 16;
    hash ^= data[sizeof (uint16_t)] << 18;
    hash += hash >> 11;
    break;
  case 2: hash += get16bits (data);
    hash ^= hash << 11;
    hash += hash >> 17;
    break;
  case 1: hash += *data;
    hash ^= hash << 10;
    hash += hash >> 1;
  }
  /* Force "avalanching" of final 127 bits */
  hash ^= hash << 3;
  hash += hash >> 5;
  hash ^= hash << 4;
  hash += hash >> 17;
  hash ^= hash << 25;
  hash += hash >> 6;
  return hash % tablesize;
}

