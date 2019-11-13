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
	printf("making a LH, testing open and close\n");
	lhash_t* locked = lhopen(10);
	if (locked == NULL){
		exit(EXIT_FAILURE);
	}
	lhclose(locked);
	exit(EXIT_SUCCESS);
}
