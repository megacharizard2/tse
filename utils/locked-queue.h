/* locked-queue.h --- locked queue
 * 
 * 
 * Author: Samuel H. Baker
 * Created: Tue Nov 12 21:25:55 2019 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef void lqueue_t;

lqueue_t* lqopen(void);

int32_t lqput(lqueue_t *lqp, void *elementp);

void* lqget(lqueue_t *lqp);

void lqapply(lqueue_t *lqp, void (*fn)(void* elementp));

void* lqsearch(lqueue_t *lqp, bool (*searchfn)(void* elementp, const void* keyp),const void* skeyp);

void lqclose(lqueue_t *lqp);
