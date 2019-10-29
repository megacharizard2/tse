#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <webpage.h>
#include <hash.h>
#include <tools.h>
#include <queue.h>
#include <ctype.h>

static char* NormalizeWord(char* word){
  char wordarray[strlen(word)+1];
  char* lowered=assertp(malloc(sizeof(wordarray)+1),"allocating space for lowered char*");
  for (int x=0;x<sizeof(wordarray);x++){
    wordarray[x]=tolower(word[x]);
  }
  strcpy(lowered,wordarray);
  return lowered;
}

static void read_file(FILE *fp,int docid,hashtable_t *ht); 

int loadcrawlerdir(char* dirname,int docid,hashtable_t* ht){
  char* slash= "/";
  char name[12];
  sprintf(name,"%d",docid);
  char* dircopy = assertp(malloc(strlen(dirname)+strlen(slash)+strlen(name)+1), "dir copy");
  strcpy(dircopy,dirname);
  char *pathname=strcat(dircopy,slash);
  char *path=strcat(pathname,name);
  FILE *fp;
  fp=fopen(path,"r");
  if (fp == NULL){
    free(dircopy);
    return 1;
  }
  else{
    read_file(fp,docid,ht);
    fclose(fp);
    free(dircopy);
    loadcrawlerdir(dirname,docid+1,ht);
  }
  return 0;
}

static bool searchfn(void* elementp, const void* searchkeyp) {
  wrapper_t* wrapper=(wrapper_t*)elementp;
  const char* str=searchkeyp;
  char* name=wrapper_getword(wrapper);
  if (strcmp(name,str) == 0){
    return true;
  }
  return false;
}

static void read_file(FILE *fp,int docid,hashtable_t *ht){
  char *url=freadlinep(fp);
  char *depth=freadlinep(fp);
  char *htmllen=freadlinep(fp); 
  printf("%s\n", htmllen);
  char *html=freadfilep(fp); 
  if (url != NULL && depth != NULL && html != NULL){
    int dep=atoi(depth);
    webpage_t* webpage=assertp(webpage_new(url,dep,html),"error creating webpage");
    int pos = 0;
    char *result;
    while ((pos = webpage_getNextWord(webpage, pos, &result)) > 0) {
      if (strlen(result)>=3){
        counters_t *ct=counters_new();
        char* lowered=NormalizeWord(result);
        wrapper_t* wrapper=wrapper_new(lowered,ct);
        wrapper_t* inserted=hsearch(ht,&searchfn,lowered,strlen(lowered));
        if (inserted != NULL){
          wrapper_delete(wrapper);
        }
        else{
           hput(ht,wrapper,lowered,strlen(lowered));
        }
        wrapper_t *inhash=hsearch(ht,&searchfn,lowered,strlen(lowered));
        if (inhash != NULL){
          counters_t *itemct=wrapper_getcounter(inhash);
          counters_add(itemct,docid);
        }
        free(lowered);
      }
      free(result);
    }
    webpage_delete(webpage);
  }
  free(url);
  free(depth);
  free(htmllen);
}
