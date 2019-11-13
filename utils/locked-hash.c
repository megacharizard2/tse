/* locked-hash.c --- locked hash
1;5202;0c * 
 * 
 * Author: Samuel H. Baker
 * Created: Tue Nov  5 17:30:15 2019 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */
#include "locked-hash.h"
#include <pthread.h>
#include <stdint.h>
#include "string.h"
#include <stdlib.h>
#include <hash.h>
#include <queue.h>

pthread_mutex_t m;

typedef struct lhash{
	hashtable_t *h;

} lhash_s;

lhash_t *lhopen(uint32_t lhsize){
	pthread_mutex_init(&m,NULL);
	lhash_s* lockedH= (lhash_s*)malloc(sizeof(lhash_s*));
	if (lockedH == NULL) {
		return NULL;
	}
	else{
		hashtable_t* hold = hopen(lhsize);
		lockedH->h=hold;
		lhash_t* lockH = (lhash_t*)lockedH;
		return lockH;
	}
}

int32_t lhput(lhash_t *lhtp, void *ep, const char *key, int keylen){
	pthread_mutex_lock(&m);
	lhash_s* lockH = (lhash_s*)lhtp;
	uint32_t returned=hput(lockH->h,ep,key,keylen);
	pthread_mutex_unlock(&m);
	return returned;
}

void lhapply(lhash_t *lhtp, void (*fn)(void* ep)){
	pthread_mutex_lock(&m);
	lhash_s* lockedH = (lhash_s*)lhtp;
	happly(lockedH->h,(fn));
	pthread_mutex_unlock(&m);
}

void *lhsearch(lhash_t *lhtp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen){
	pthread_mutex_lock(&m);
	lhash_s* lockedH = (lhash_s*) lhtp;
	void* searched = hsearch(lockedH->h,searchfn,key,keylen);
	pthread_mutex_unlock(&m);
	return searched;
}

void *lhremove(lhash_t* lhtp,
							 bool (*searchfn)(void* elementp, const void* searchkeyp),
							 const char *key,
							 int32_t keylen) {
	pthread_mutex_lock(&m);
	lhash_s* lockedH = (lhash_s*)lhtp;
	void* removed = hremove(lockedH->h,searchfn,key,keylen);
	pthread_mutex_unlock(&m);
	return removed;
}

void lhclose(lhash_t* lhtp) {
	pthread_mutex_lock(&m);
	lhash_s* lockedH = (lhash_s*)lhtp;
	hclose(lockedH->h);
	free(lockedH);
	pthread_mutex_unlock(&m);
	pthread_mutex_destroy(&m);
}
