/* locked-queue.h --- 
 * 
 * 
 * Author: Jack F. Vasu
 * Created: Mon Nov  4 19:35:10 2019 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdint.h>
#include <stdbool.h>

typedef void lqueue_t;

lqueue_t* lqopen(void);

int32_t lqput(lqueue_t *lqp, void *elementp);

void* lqget(lqueue_t *lqp);

void lqapply(lqueue_t *lqp, void (*fn)(void* elementp));

void* lqsearch(lqueue_t *lqp, bool (*searchfn)(void* elementp, const void* keyp), const void* skeyp);

void lqclose(lqueue_t *lqp);
