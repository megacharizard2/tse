/* lhashtest2.c --- second test
 * 
 * 
 * Author: Samuel H. Baker
 * Created: Tue Nov 12 23:02:22 2019 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */
#include "locked-hash.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct person {
	char* name;
	int number;
} person_t;

bool searchfn(void* elementp, const void* searchkeyp) {
	person_t* person=elementp;
	char* name=person->name;
	const char* str=searchkeyp;
	if (strcmp(name,str) ==0){
		return true;
	}
	return false;
}

void printcar(void* ep){
	person_t* person=ep;
	printf("person=%s\n",person->name);
}

int main() {
	printf("Testing removing person data structures from lhashtable\n");
	printf("opening lhashtable with size 100\n");
	lhash_t* locked = lhopen(100);
	printf("Putting person data  structure in lhashtable with eky as it's plate\n");
	char* two="Satch";
	person_t secondperson={"Satch", 3000};
	char* three="Jack";
	person_t thirdperson={"Jack", 1600};
	int32_t returncode= lhput(locked,&secondperson,two,strlen(two));
	if (returncode != 0) {
		exit(EXIT_FAILURE);
	}
	int32_t returncode2=lhput(locked,&thirdperson,three,strlen(three));
	if (returncode2 != 0) {
		exit(EXIT_FAILURE);
	}
	printf("Putting in satch and jack \n");
	person_t* personremove=lhremove(locked,&searchfn,two,strlen(two));
	printcar(personremove);
	printf("applying func to print names of all people in table\n");
	lhapply(locked,&printcar);
	lhclose(locked);
	exit(EXIT_SUCCESS);
}
