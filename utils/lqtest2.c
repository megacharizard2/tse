/* lqtest1.c --- 
 * 
 * 
 * Author: Jack F. Vasu
 * Created: Mon Nov  4 16:59:27 2019 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */
#include "locked-queue.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	printf("making a LQ, testing lqclose\n");
	lqueue_t* locked = lqopen();
	int x = 2;
	int* ptr = &x;
	lqput(locked, ptr);
	printf("putting another element in lockedQ\n");
	int y = 2;
	int* ptr2 = &y;
	lqput(locked, ptr2);
	lqclose(locked);
	exit(EXIT_SUCCESS);
}
