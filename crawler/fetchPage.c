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
		char* URL =webpage_getURL(sample);
		char* HTML = webpage_getHTML(sample);
		int depth = webpage_getDepth(sample);
		printf("the url is %s, html is %s, depth is %d", URL, HTML, depth);
		free(sample);
		free(URL);
		free(HTML);
		exit(EXIT_SUCCESS);
	}
	else{
		printf("something went wrong");
	}
	
	exit(EXIT_SUCCESS);
}
