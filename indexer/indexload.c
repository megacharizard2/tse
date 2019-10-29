#include <stdio.h>
#include "indexio.h"
#include "hash.h"
#include "tools.h"

FILE* fpout;

void counterprint(void* arg,const int key,int count){
  FILE* fp=arg;
  fprintf(fp,"%d %d ",key,count);
}

void print(void* ep){
  wrapper_t* wrapper=(wrapper_t*)ep;
  char* word=wrapper_getword(wrapper);
  counters_t* ctr=wrapper_getcounter(wrapper);
  fprintf(fpout,"%s ",word);
  counters_iterate(ctr,fpout,&counterprint);
  fprintf(fpout,"\n");
}

void deletewrapper(void* ep){
  wrapper_t* wrapper=(wrapper_t*)ep;
  if (wrapper != NULL){
     wrapper_delete(wrapper);
  }
}

int main(int argc,char* argv[]){
  if (argc < 3){
    printf("need 2 arguments");
    return 1;
  }
  FILE* fp;
  fp=fopen(argv[1],"r");
  if (fp == NULL){
    printf("could not open file %s for reading\n",argv[1]);
    return 1;
  }
  fpout=fopen(argv[2],"w");
  if (fp == NULL){
    printf("could not open file %s for writing\n",argv[2]);
    return 1;
  }
  hashtable_t* ht=loadindexfile(fp);
  happly(ht,print);
  happly(ht, deletewrapper);
  hclose(ht);
  fclose(fp);
  fclose(fpout);
}
