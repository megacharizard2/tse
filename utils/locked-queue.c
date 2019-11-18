/* locked-queue.c --- Make a locked queue
 * 
 * 
 * Author: Samuel H. Baker
 * Created: Thu Oct 31 17:06:48 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include "locked-queue.h"
#include "queue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t m;

typedef struct lqueue{
	queue_t *q;

} lqueue_s;

lqueue_t* lqopen(void) {
	pthread_mutex_init(&m,NULL);
	lqueue_s* lockedQ= (lqueue_s*)malloc(sizeof(lqueue_s*));
	if (lockedQ == NULL) {
		return NULL;
	}
	else{
		queue_t* hold = qopen();
		lockedQ->q=hold;
		lqueue_t* lq = (lqueue_t*)lockedQ;
		return lq;
	}
}
int32_t lqput(lqueue_t *lqp, void *elementp) {
	pthread_mutex_lock(&m);
	lqueue_s* lqueue = (lqueue_s*)lqp;
	int32_t toReturn;
	if (qput(lqueue->q,elementp)==1){
		toReturn=1;
	}
	else{
		toReturn=0;
	}
	pthread_mutex_unlock(&m);
	return toReturn;
}

void* lqget(lqueue_t *lqp) {
	pthread_mutex_lock(&m);
	lqueue_s* lqueue = (lqueue_s*)lqp;
	void* item = qget(lqueue->q);
	pthread_mutex_unlock(&m);
	return item;
}

void lqapply(lqueue_t *lqp, void (*fn)(void* elementp)) {
	pthread_mutex_lock(&m);
	lqueue_s* lqueue = (lqueue_s*)lqp;
	qapply(lqueue->q,(fn));
	pthread_mutex_unlock(&m);
}

void* lqsearch(lqueue_t *lqp, bool (*searchfn)(void* elementp,const void* keyp),const void* skeyp) {
	pthread_mutex_lock(&m);
	lqueue_s* lqueue = (lqueue_s*)lqp;
	void* search = qsearch(lqueue->q, searchfn,skeyp);
	pthread_mutex_unlock(&m);
	return search;
}

void lqclose(lqueue_t *lqp) {
	pthread_mutex_lock(&m);
	lqueue_s* lqueue = (lqueue_s*)lqp;
	qclose(lqueue->q);
	free(lqueue);
 	pthread_mutex_unlock(&m);
	pthread_mutex_destroy(&m);
}
