#include <stdio.h>
#include "indexio.h"
#include "hash.h"
#include "tools.h"

FILE* fp;
int numwords=0;
void counterprint(void* arg,const int key,int count){
  FILE* fp=arg;
  fprintf(fp,"%d %d ",key,count);
}

void print(void* ep){
  wrapper_t* wrapper=(wrapper_t*)ep;
  char* word=wrapper_getword(wrapper);
  counters_t* ctr=wrapper_getcounter(wrapper);
  fprintf(fp,"%s ",word);
  counters_iterate(ctr,fp,&counterprint);
  fprintf(fp,"\n");
}

void countercount(void* arg,const int key,int count){
  int* numwords=(int*)arg;
  *numwords+=count;
}

void getcount(void* ep){
  wrapper_t* wrapper=(wrapper_t*)ep;
  counters_t* ctr=wrapper_getcounter(wrapper);
  counters_iterate(ctr,&numwords,&countercount);
}

void deletewrapper(void* ep){
  wrapper_t* wrapper=(wrapper_t*)ep;
  if (wrapper != NULL){
     wrapper_delete(wrapper);
  }
}
bool isCrawlerDirectory(char* c){
  char *filename="/crawler";
  char* dircopy = assertp(malloc(strlen(c)+strlen(filename)+1), "dircopy");
  strcpy(dircopy,c);
  char *pathname=strcat(dircopy,filename);
  FILE* fp;
  fp=fopen(pathname,"r");
  if (fp == NULL){
    printf("%s is not crawler produced or doesn't exist\n",c);
    free(dircopy);
    return false;
  }
  printf("%s is crawler produced\n",c);
  free(dircopy);
  fclose(fp);
  return true;
}

int main(int argc,char* argv[]){
  if (argc < 3){
    printf("two arguments required\n");
    return 1;
  }
  char* directory=argv[1];
  char* indexfilename=argv[2];
  bool iscrawlerproduced=isCrawlerDirectory(directory);
  if (iscrawlerproduced == false){
    return 1;
  }
  fp=fopen(indexfilename,"w");
  if (fp == NULL){
    printf("could not open %s for writing\n",indexfilename);
    return 1;
  }
  hashtable_t* ht=hopen(900);
  if (ht == NULL){
    printf("Hashtable could not be created\n");
    return 1;
  }
  loadcrawlerdir(directory,1,ht);
  happly(ht,print);
  happly(ht,getcount);
  happly(ht,deletewrapper);
  hclose(ht);
  fclose(fp);
  return 0;
}
