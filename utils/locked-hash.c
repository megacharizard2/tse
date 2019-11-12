/* locked-hash.c --- locked hash
 * 
 * 
 * Author: Samuel H. Baker
 * Created: Tue Nov  5 17:30:15 2019 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <pthread.h>
#include <stdint.h>
#include "string.h"
#include <stdlib.h>
#include <hash.h>
#include <queue.h>

pthread_mutex_t m;

typedef struct lhash{
	hashtable_t *h;

} lhash_t;

lhash_t *lhopen(uint32_t lhsize){
	pthread_mutex_init(&m,NULL);
	lhash_t* lockedH= malloc(sizeof(lhash_t*));
	if (lockedH == NULL) {
		return NULL;
	}
	else{
		hashtable_t* hold = malloc(sizeof(hashtable_t*));
		hold = hopen(lhsize);
		lockedH->h=hold;
		return lockedH;
	}
}

int32_t lhput(lhash_t *lhtp, void *ep, const char *key, int keylen){
	pthread_mutex_lock(&m);
	uint32_t returned=hput(lhtp->h,ep,key,keylen);
	pthread_mutex_unlock(&m);
	return returned;
}

void lhapply(lhash_t *lhtp, void (*fn)(void* ep)){
	pthread_mutex_lock(&m);
	happly(lhtp->h,(fn));
	pthread_mutex_unlock(&m);
}

void lhsearch(lhash_t *lhtp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen){
	pthread_mutex_lock(&m);
	hsearch(lhtp->h,searchfn,key,keylen);
	pthread_mutex_unlock(&m);
}

void lhremove(lhash_t* lhtp,
							 bool (*searchfn)(void* elementp, const void* searchkeyp),
							 const char *key,
							 int32_t keylen) {
	pthread_mutex_lock(&m);
	hremove(lhtp->h,searchfn,key,keylen);
	pthread_mutex_unlock(&m);
}

void lhclose(lhash_t* lhtp) {
	pthread_mutex_lock(&m);
	hclose(lhtp->h);
	pthread_mutex_unlock(&m);
	pthread_mutex_destroy(&m);
}
