#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "locked-queue.h"
#include "locked-hash.h"
#include "webpage.h"
#include <pthread.h>

typedef struct params {
	int depth;
	char* seedURL;
	char* dirname;
} params_t;

void *
assertp(void *p, const char *message)
{
  if (p == NULL) {
    fprintf(stderr, "MALLOC FAILED: %s\n", message);
    exit(EXIT_FAILURE);
  }
  return p;
}

bool isdir(char *c){
  char *filename="/crawler";
  char* dircopy = assertp(malloc(strlen(c)+strlen(filename)+1), "dict copy");
  strcpy(dircopy,c);
  char *pathname=strcat(dircopy,filename);
  FILE *fp;
  fp=fopen(pathname,"w");
  if (fp== NULL){
    free(dircopy);
    return false;
  }
  else{
    fclose(fp);
    free(dircopy);
    return true;
  }
  return true;
}

inline static void logr(const char *word, const int depth, const char *url)
{
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}

int32_t pagesaver(webpage_t *page,int id,char* dirname){
  char name[12];
  sprintf(name,"%d",id);
  char* slash="/";
  char* dircopy = assertp(malloc(strlen(dirname)+strlen(slash)+strlen(name)+1), "dict copy");
  strcpy(dircopy,dirname);
  char *pathname=strcat(dircopy,slash);
  char *path=strcat(pathname,name);
  FILE* fp;
  fp=fopen(path,"w");
  if (fp == NULL){
    printf("the file pointer is NULL");
    return 1;
  }
  char *URL=webpage_getURL(page);
  int depth=webpage_getDepth(page);
  int htmllen=webpage_getHTMLlen(page);
  char *HTML=webpage_getHTML(page);
  fprintf(fp,"%s\n%d\n%d\n%s\n",URL,depth,htmllen,HTML);
  fclose(fp);
  free(dircopy);
  return 0;
}

bool searchfn(void* elementp, const void* searchkeyp) {
  char* name=(char*)elementp;
  const char* str=searchkeyp;
  if (strcmp(name,str) == 0){
    return true;
  }
  return false;
}

void printfn(void* elementp){
  char* elemen=(char*)elementp;
  printf("%s\n",elemen);
}

void qprintfn(void* elementp){
  webpage_t* page=(webpage_t*)elementp;
  printf("%s\n",webpage_getURL(page));
}

void deletestring(void* elementp){
  char* elemen=(char*)elementp;
  free(elemen);
}

void crawler(char* seedURL,int maxdepth,char* dirname){
  char* urlcopy = assertp(malloc(strlen(seedURL)+1), "url copy\n");
  strcpy(urlcopy,seedURL);
  webpage_t *web = webpage_new(urlcopy, 0, NULL); 
  lqueue_t *tocrawl=assertp(lqopen(),"error creating queue\n"); 
  const int hashsize=50;
  lhash_t *seenURLs=assertp(lhopen(hashsize),"error creating hashtable");
  lqput(tocrawl,web);
  lhput(seenURLs,urlcopy,urlcopy,strlen(urlcopy));
  webpage_t* page;
	int pagename=0;
  while ((page=lqget(tocrawl)) != NULL){
    bool fetchsuccess=webpage_fetch(page);
    if (fetchsuccess != true){
      printf("could not extract HTML from page %s\n",urlcopy);
      webpage_delete(page);
      continue;
    }
    logr("Fetched",webpage_getDepth(page),webpage_getURL(page));
    pagename+=1;
    pagesaver(page,pagename,dirname);
    int depth=webpage_getDepth(page);
    if (depth<maxdepth){
      int pos=0;
      char* result;
      logr("Scanning",webpage_getDepth(page),webpage_getURL(page));
      while ((pos = webpage_getNextURL(page,pos,&result)) > 0) {
        logr("Found",webpage_getDepth(page),result);
        if (NormalizeURL(result)){
          if (!IsInternalURL(result)){
            logr("IgnExtrn",webpage_getDepth(page),result);
          }
          if (IsInternalURL(result)){
            char* addedornot=lhsearch(seenURLs,&searchfn,result,strlen(result));
            if (addedornot != NULL){
              logr("IgnDupl",webpage_getDepth(page),result);
            }
            if (addedornot == NULL){
              char* urlcpy = assertp(malloc(strlen(result)+1), "result copy");
              strcpy(urlcpy,result);
              lhput(seenURLs,urlcpy,urlcpy,strlen(urlcpy));
              webpage_t *newpage = webpage_new(urlcpy, depth+1, NULL); 
              lqput(tocrawl,newpage);
              logr("Added",webpage_getDepth(page),result);
            }
          }
        }
        else{
          fprintf(stderr, "%s cannot be normalized\n", result);
        }
        free(result);
      } 
    } 
    webpage_delete(page);
  }
  lhapply(seenURLs,printfn);
  lhclose(seenURLs);
  lqclose(tocrawl);
}

void* crawlerhelper(void* param) {
	params_t parameters = (params_t)param;
	crawler(parameters.depth,parameters.seedURL,parameters.dirname);
}

int main(int argc,char* argv[]){
  if (argc != 5){
    printf("There must be 5 arguments");
    printf("usage: crawler <seedurl><pagedir> <maxdepth>\n");
    return 1;
  }
  char* seedURL=argv[1];
  if (!NormalizeURL(seedURL)) {
    fprintf(stderr, "%s cannot be normalized\n", seedURL);
    printf("usage: crawler <seedurl><pagedir> <maxdepth>\n");
    return 2;
  }
  char* pageDirectory=argv[2];
  bool dir=isdir(pageDirectory);
  if (dir == false){
    printf("Directory does not exist or is not writeable\n");
    printf("usage: crawler <seedurl><pagedir> <maxdepth>\n");
    return 1;
  }
	int threads=atoi(argv[4]);
	if (threads == 0) {
		printf("Final argument must be an integer greater than 0");
		return 2;
	}
	int threadid = 0;
	int* array;
	array = malloc(threads * (sizeof(int));
  int depth=atoi(argv[3]);
	int i;
	params_t param={depth,seedURL,pageDirectory};
	for (i=0; i<threads; i++) {
		array[i] = i + 1;
		pthread_create(&array[i], NULL, crawlerHelper,param); 
	}
  /*crawler(seedURL,depth,pageDirectory);*/
}
