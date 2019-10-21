/* fetchPage.c --- 
 * 
 * 
 * Author: Jack F. Vasu
 * Created: Thu Oct 17 16:37:22 2019 (-0400)
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
int main(int argc, char* argv[]){

	webpage_t* sample= webpage_new("https://thayer.github.io/engs50/", 0, NULL);
	if (webpage_fetch(sample)){
		char* URL = webpage_getURL(sample);
		char* HTML = webpage_getHTML(sample);
		int depth = webpage_getDepth(sample);
		printf("the url is %s, html is %s, depth is %d\n", URL, HTML, depth);
		int pos=0;
		char *resultURL;
		queue_t *q=qopen();
		while ((pos=webpage_getNextURL(sample, pos, &resultURL)) > 0){
			printf("found url: %s\n", resultURL);
			if(IsInternalURL(resultURL)){//takes a pointer URL and bools
				printf("the url %s is internal\n", resultURL);		
				qput(q,yresultURL);
			}
			else{
				printf("the url %s is external\n", resultURL);
			}
			printf("\n");
			free(resultURL);
		}
		char *hold;
		while ((hold=qget(q)) != NULL){
			printf("url is %s\n", hold);
		}
		free(hold);
		qclose(q);
		free(sample);
		free(URL);
		free(HTML);
		exit(EXIT_SUCCESS);
		}
	else{
		printf("page does not exist");
		exit(EXIT_SUCCESS);
	}
	
	exit(EXIT_SUCCESS);
}
