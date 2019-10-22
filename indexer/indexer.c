/* indexer.c --- indexer
 * 
 * 
 * Author: Samuel H. Baker
 * Created: Tue Oct 22 17:37:48 2019 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <queue.h>
#include <hash.h>
#include <webpage.h>

void *
assertp(void *p, const char *message) {
	if (p == NULL) {
		fprintf(stderr, "MALLOC FAILED: %s\n", message);
		exit(EXIT_FAILURE);
	}
	return p;
}
