/* lhashtest1.c --- 
 * 
 * 
 * Author: Samuel H. Baker
 * Created: Tue Nov 12 17:32:23 2019 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */
#include "locked-hash.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	printf("making a LH, testing put and get\n");
	lhash_t* locked = lhopen();
	printf("putting an element in lockedH\n");
	int x = 2;
	int* ptr = &x;
	lhput(locked
