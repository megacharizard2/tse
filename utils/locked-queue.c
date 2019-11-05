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
#include <queue.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t m;

typedef struct lqueue{
	queue_t *q;

} lqueue_t;

lqueue_t* lqopen(void) {
	pthred_mutex_init(&m,NULL);
	lqueue_t* lockedQ= malloc(sizeof(lqueue_t*));
	if (lockedQ == NULL) {
		return NULL;
	}
	else{
		queue_t* hold = malloc(sizeof(queue_t*));
		hold = qopen();
		lockedQ->q=hold;
		return lockedQ;
	}
}
void lqput(lqueue_t *lqp, void *elementp) {
	pthread_mutex_lock(&m);
	qput(lqp->q,elementp);
	pthread_mutex_unlock(&m);
}

void* lqget(lqueue_t *lqp) {
	pthread_mutex_lock(&m);
	void* item = qget(lqp->q);
	pthread_mutex_unlock(&m);
	return item;
}

void lqapply(lqueue_t *lqp, void (*fn)(void* elementp)) {
	pthread_mutex_lock(&m);
 	qapply(lqp->q,(fn));
	pthread_mutex_unlock(&m);
}

void* lqsearch(lqueue_t *lqp, bool (*searchfn)(void* elementp,const void* keyp),const void* skeyp) {
	pthread_mutex_lock(&m);
	void* search = qsearch(lqp->q, searchfn,skeyp);
	pthread_mutex_unlock(&m);
	return search;
}

void lqclose(lqueue_t *lqp) {
	pthread_mutex_lock(&m);
	qclose(lqp->q);
	pthread_mutex_unlock(&m);
	pthread_mutex_destroy(&m);
}
