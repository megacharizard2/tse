#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <queue.h>
#include <hash.h>
#include <webpage.h>

int32_t pagesave(webpage_t *pagep,int id,char* dirnm){
  char name[12];
  sprintf(name,"%d",id);
  char* slash="/";
  char* dircopy = assertp(malloc(strlen(dirnm)+strlen(slash)+strlen(name)+1), "dict copy");
  strcpy(dircopy,dirnm);
  char *pathname=strcat(dircopy,slash);
  char *path=strcat(pathname,name);
  FILE* fp;
  fp=fopen(path,"w");
  if (fp == NULL){
    printf("the file pointer is NULL");
    return 1;
  }
  char *URL=webpage_getURL(pagep);
  int depth=webpage_getDepth(pagep);
  int htmllen=webpage_getHTMLlen(pagep);
  char *HTML=webpage_getHTML(pagep);
  fprintf(fp,"%s\n%d\n%d\n%s\n",URL,depth,htmllen,HTML);
  fclose(fp);
  free(dircopy);
  return 0;
}

webpage_t *pageload(int id, char *dirnm){
  printf("hello");
} 
