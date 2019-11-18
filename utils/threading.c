#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "locked-queue.h"
#include "locked-hash.h"
#include "webpage.h"
#include <pthread.h>

int stillgoing;
int pagename = 0;

pthread_mutex_t idlock;
pthread_mutex_t stillgoinglock;
pthread_mutex_t hashlock;

typedef struct params {
	int depth;
	char* seedURL;
	char* dirname;
	lqueue_t* lqueue;
	lhash_t* lhash;
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
	printf("does it enter");
  char *filename="/crawler";
	printf("line1");
  char* dircopy = assertp(malloc(strlen(c)+strlen(filename)+1), "dict copy");
	printf("line2");
	strcpy(dircopy,c);
	printf("line3");
  char *pathname=strcat(dircopy,filename);
  FILE *fp;
  fp=fopen(pathname,"w");
	printf("file opened\n");
  if (fp== NULL){
    free(dircopy);
		printf("messing up the if\n");
    return false;
  }
  else{
    fclose(fp);
    free(dircopy);
		printf("messing up the else\n");
    return true;
  }
	printf("gets through\n");
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
    printf("the file pointer is NULL\n");
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

void* threader(void* parameter){
	printf("starts threader going\n");
	bool isinactive = false;
	params_t* param = (params_t*)parameter;
	char* seedURL = param->seedURL;
	char* dirname = param->dirname;
	int maxdepth = param->depth;
	char* urlcopy = assertp(malloc(strlen(seedURL)+1), "url copy\n");
  strcpy(urlcopy,seedURL);
  webpage_t *web = webpage_new(urlcopy, 0, NULL); 
  lqueue_t *tocrawl=param->lqueue; 
	lhash_t* seenURLs=param->lhash;
	printf("housekeeping done with\n");
	if(lqget(tocrawl)==NULL){
		printf("no queue to crawl\n");
		if((lqput(tocrawl,web))==1){
			printf("entered base page  into queue\n");
		}
		if((lhput(seenURLs,urlcopy,urlcopy,strlen(urlcopy)))==1){
				printf("entered into hash\n");
		}
	}
  webpage_t* page;
  while (stillgoing != 0){
		printf("enters big dog loop\n");
		if ((page = lqget(tocrawl)) != NULL) {
			if(isinactive==true){
				pthread_mutex_lock(&stillgoinglock);
				stillgoing += 1;
				pthread_mutex_unlock(&stillgoinglock);
				isinactive=false;
			}
			bool fetchsuccess=webpage_fetch(page);
			if (fetchsuccess != true){
				printf("could not extract HTML from page %s\n",urlcopy);
				webpage_delete(page);
				continue;
			}
			logr("Fetched",webpage_getDepth(page),webpage_getURL(page));
			pthread_mutex_lock(&idlock);
			pagename+=1;
			pthread_mutex_unlock(&idlock);
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
							pthread_mutex_lock(&hashlock);
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
							pthread_mutex_unlock(&hashlock);
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
		else {
			if(isinactive == false){
				isinactive = true;
				pthread_mutex_lock(&stillgoinglock);
				stillgoing -= 1;
				pthread_mutex_unlock(&stillgoinglock);

			}
		}
	}
	return NULL;
}
	
int main(int argc,char* argv[]){
	printf("in main\n");
  if (argc != 5){
    printf("There must be 5 arguments\n");
    printf("usage: crawler <seedurl><pagedir> <maxdepth>\n");
    return 1;
  }
	printf("arguments OK\n");
  char* seedURL=argv[1];
  if (!NormalizeURL(seedURL)) {
    fprintf(stderr, "%s cannot be normalized\n", seedURL);
    printf("usage: crawler <seedurl><pagedir> <maxdepth>\n");
    return 2;
  }
	printf("the page directory is %s\n",argv[2]);
  char* pageDirectory=argv[2];
  bool dir=isdir(pageDirectory);
	printf("isdir fine");
	if (dir == false){
    printf("Directory does not exist or is not writeable\n");
    printf("usage: crawler <seedurl><pagedir> <maxdepth>\n");
    return 1;
  }
	printf("pagedir fine\n");
	int threads=atoi(argv[4]);
	printf("made thread\n");
	stillgoing=threads;
	if (threads == 0) {
		printf("Final argument must be an integer greater than 0\n");
		return 2;
	}
	lqueue_t* lqueue = lqopen();
	const int hashsize=50;
	lhash_t* lhash = lhopen(hashsize);
  pthread_t thread[threads];
  int depth=atoi(argv[3]);
	int i;
	/*struct params_t* param=malloc(sizeof(params_t));*/
	params_t* param=(params_t*)malloc(sizeof(params_t));
	printf("malloced\n");
	param->depth=depth;
	param->seedURL=seedURL;
	param->dirname=pageDirectory;
	param->lqueue=lqueue;
	param->lhash=lhash;
	printf("made struct\n");
	for (i=0; i<threads; i++) {
		printf("trying to make thread\n");
		if((pthread_create(&thread[i], NULL,threader,(void *)param))!=0){
			printf("failed to make thread\n");
		}
		else{
		printf("made thread\n");
		}
 	}
	printf("gets out of making threads\n");
	int j;
	for (j=0; j<threads; j++) {
		if((pthread_join(thread[j],NULL))!=0){
		printf("failed to join thread\n");
		}
		else{printf("joined thread\n");}
	}
	fflush(stdout);
	lhapply(lhash,printfn);
	lhapply(lhash,deletestring);
	lhclose(lhash);
	lqclose(lqueue);
}
