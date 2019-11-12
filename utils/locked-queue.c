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
<<<<<<< HEAD
#include <queue.h>
=======
#include "locked-queue.h"
#include "queue.h"
>>>>>>> locked
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t m;

typedef struct lqueue{
	queue_t *q;

<<<<<<< HEAD
} lqueue_t;

lqueue_t* lqopen(void) {
	pthred_mutex_init(&m,NULL);
	lqueue_t* lockedQ= malloc(sizeof(lqueue_t*));
=======
} lqueue_s;

lqueue_t* lqopen(void) {
	pthread_mutex_init(&m,NULL);
	lqueue_s* lockedQ= (lqueue_s*)malloc(sizeof(lqueue_s*));
>>>>>>> locked
	if (lockedQ == NULL) {
		return NULL;
	}
	else{
<<<<<<< HEAD
		queue_t* hold = malloc(sizeof(queue_t*));
		hold = qopen();
		lockedQ->q=hold;
		return lockedQ;
	}
}
void lqput(lqueue_t *lqp, void *elementp) {
	pthread_mutex_lock(&m);
	qput(lqp->q,elementp);
=======
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
	return toReturn;
>>>>>>> locked
	pthread_mutex_unlock(&m);
}

void* lqget(lqueue_t *lqp) {
	pthread_mutex_lock(&m);
<<<<<<< HEAD
	void* item = qget(lqp->q);
	pthread_mutex_unlock(&m);
	return item;
=======
	lqueue_s* lqueue = (lqueue_s*)lqp;
	return qget(lqueue->q);
	pthread_mutex_unlock(&m);
>>>>>>> locked
}

void lqapply(lqueue_t *lqp, void (*fn)(void* elementp)) {
	pthread_mutex_lock(&m);
<<<<<<< HEAD
 	qapply(lqp->q,(fn));
=======
	lqueue_s* lqueue = (lqueue_s*)lqp;
	qapply(lqueue->q,(fn));
>>>>>>> locked
	pthread_mutex_unlock(&m);
}

void* lqsearch(lqueue_t *lqp, bool (*searchfn)(void* elementp,const void* keyp),const void* skeyp) {
	pthread_mutex_lock(&m);
<<<<<<< HEAD
	void* search = qsearch(lqp->q, searchfn,skeyp);
=======
	lqueue_s* lqueue = (lqueue_s*)lqp;
	void* search = qsearch(lqueue->q, searchfn,skeyp);
>>>>>>> locked
	pthread_mutex_unlock(&m);
	return search;
}

void lqclose(lqueue_t *lqp) {
	pthread_mutex_lock(&m);
<<<<<<< HEAD
	qclose(lqp->q);
	pthread_mutex_unlock(&m);
=======
	lqueue_s* lqueue = (lqueue_s*)lqp;
	qclose(lqueue->q);
	free(lqueue);
 	pthread_mutex_unlock(&m);
>>>>>>> locked
	pthread_mutex_destroy(&m);
}
