#include <stdio.h>
#include "indexio.h"
#include "hash.h"
#include "tools.h"

void deletewrapper(void* ep){
  wrapper_t* wrapper=(wrapper_t*)ep;
  if (wrapper != NULL){
    wrapper_delete(wrapper);
  }
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

void counters_union(counters_t* ct1, counters_t* ct2);
static void counters_union_helper(void* arg,const int key,int count);

struct twocts {
  counters_t *result;
  counters_t *another;
};

void counters_intersect(counters_t* ct1, counters_t* ct2); 
void intersect_helper(void *arg, const int key, const int count);

static inline int min(const int a, const int b) {
  return (a < b ? a : b);
}

void counters_intersect(counters_t* ct1, counters_t* ct2)
{
  assertp(ct1, "counters 1 invalid");
  assertp(ct2, "counters 2 invalid");

  struct twocts args = {ct1, ct2}; 
  counters_iterate(ct1, &args, intersect_helper);
}

void intersect_helper(void *arg, const int key, const int count)
{
  struct twocts *two = arg; 

  counters_set(two->result, key, min(count, counters_get(two->another, key)));
}

void counters_union(counters_t* ct1, counters_t* ct2)
{
  counters_iterate(ct2,ct1,counters_union_helper);
}

static void counters_union_helper(void* arg,const int key,int count){
  counters_t* ct1=arg;
  int countct1=counters_get(ct1,key);
  if (countct1 ==0 && count>0){
    counters_set(ct1,key,count);
  }
  if (countct1 >0){
    counters_set(ct1,key,countct1+count);
  }
}

/* process query and get counter with processed query*/
counters_t * processquery(char* wordarray[],int array_counter,hashtable_t* index){
  counters_t* runningsum=assertp(counters_new(),"making new counter");
  counters_t* runningproduct=assertp(counters_new(),"making new counter");
  counters_t* emptycounter=assertp(counters_new(),"making new counter");
  int prodstatus=1;
  for (int x=0;x<=array_counter;x++){
    if (strcmp("and",wordarray[x]) == 0){
      continue;
    }
    if (strcmp("or",wordarray[x]) ==0){
      counters_union(runningsum,runningproduct);
      counters_delete(runningproduct);
      runningproduct=assertp(counters_new(),"making new counter");
      prodstatus=1;
    }
    if (strcmp("and",wordarray[x]) != 0 && strcmp("or",wordarray[x]) != 0){
      wrapper_t* wordwrapper=hsearch(index,&searchfn,wordarray[x],strlen(wordarray[x]));
      counters_t* wordfreq=wrapper_getcounter(wordwrapper);
      if (wordfreq == NULL){
        wordfreq=emptycounter;
      }
      if (prodstatus == 1 && x!= array_counter){
        counters_union(runningproduct,wordfreq);
        prodstatus =2;
      }
      if (prodstatus == 2 && x!= array_counter){
        counters_intersect(runningproduct,wordfreq);
      }
      if (prodstatus == 1 && x==array_counter){
        counters_union(runningproduct,wordfreq);
        counters_union(runningsum,runningproduct);
      }
      if (prodstatus ==2 && x==array_counter){
        counters_intersect(runningproduct,wordfreq);
        counters_union(runningsum,runningproduct);
      }
    }
  }
  counters_delete(emptycounter);
  counters_delete(runningproduct);
  return runningsum;
}

/* print clean validated query */
void printcleanquery(char* wordarray[],int array_counter){
  //Print clean validated query for user to see
  printf("query: ");
  for (int y=0;y<=array_counter;y++){
    printf("%s ",wordarray[y]);
  }
  printf("\n");
}
/* Check correct ordering of and,or statements*/
bool checksyntax(char* wordarray[],int counter){
  if (strcmp("and",wordarray[0])==0){
    printf("'and' cannot be first\n");
    return false;
  }
  if (strcmp("and",wordarray[counter])==0){
    printf("'and' cannot be last\n");
    return false;
  }
  if (strcmp("or",wordarray[0])==0){
    printf("'or' cannot be first\n");
    return false;
  }
  if (strcmp("or",wordarray[counter])==0){
    printf("'or' cannot be last\n");
    return false;
  }
  for (int y=1;y<counter-1;y++){
    if (strcmp("and",wordarray[y])==0){
      if (strcmp("and",wordarray[y-1])==0 || strcmp("and",wordarray[y+1])==0){
        printf("cannot have two consecutive ands\n");
        return false;
      }
      if (strcmp("or",wordarray[y-1])==0 || strcmp("or",wordarray[y+1])==0){
        printf("cannot have two consecutive ands and ors\n");
        return false;
      }
    }
    if (strcmp("or",wordarray[y])==0){
      if (strcmp("and",wordarray[y-1])==0|| strcmp("and",wordarray[y+1])==0){
        printf("cannot have two consecutive ands and ors\n");
        return false;
      }
      if (strcmp("or",wordarray[y-1])==0|| strcmp("or",wordarray[y+1])==0){
        printf("cannot have two consecutive ors\n");
        return false;
      }
    }
  }
  return true;
}

/* Deletes array of query words */
/* array consists of malloced space returned by normalizeword func */
void delete_wordarray(char* wordarray[],int array_counter){
  for (int y=0;y<=array_counter;y++){
    free(wordarray[y]);
  }
}

/* Checks for bad char in query */
/* A bad char is any character that is not a alphabet */
/* Returns true if bad char found in the string */
bool badcharcheck(char* string){
  for (int x=0;x<strlen(string);x++){
    if (isalpha(string[x]) == 0){
      printf("Error: bad character '%c' in query. \n",string[x]);
      return true;
    }
  }
  return false;
}

struct page{
  int docID;
  int score;
};

struct resultarray{
  struct page* results;
  int* index;
};

/* Makes an array of pages which hold the docID and score of matched docs */
/* Arg is a struct resultarray containing a pinter to array of pages and a index counter */
void makeresultarray(void *arg, const int key, const int count){
  struct resultarray* arraystruct=arg;
  struct page* array=arraystruct->results;
  int* index=arraystruct->index;
  struct page resultpage= {key,count};
  array[*index]= resultpage;
  *index +=1;
}

/* Counters itearator that counts number of keys(matches) in the counter */
/* The count is stored in an int pointer which is supplied as arg */
void numofmatches(void *arg, const int key, const int count){
  int* itemcount=arg;
  *itemcount+=1;
}

/* Simple implementation of insertion sort to sort array of matches*/
/* Sorts is decreasing order */
void sortmatchesarray(struct page* matchesarray,int matches){
  struct page keypage;
  int keyscore;
  int y;
  for (int x=1;x<matches;x++){
    keypage=matchesarray[x];
    keyscore=matchesarray[x].score;
    y=x-1;
    while (y>=0 && matchesarray[y].score<keyscore){
      matchesarray[y+1]=matchesarray[y];
      y=y-1;
    }
    matchesarray[y+1]=keypage;
  }
}

/* Gets URL for specific page by reading the URL from the crawled directory */
char* getpageURL(int docID,char* crawledDir){
  char dirdocID[20];
  sprintf(dirdocID,"%d",docID);
  char* slash="/";
  char* dircopy = assertp(malloc(strlen(crawledDir)+strlen(slash)+strlen(dirdocID)+1), "dircopy");
  strcpy(dircopy,crawledDir);
  char *pathname=strcat(dircopy,slash);
  char *path=strcat(pathname,dirdocID);
  FILE* fp;
  fp=fopen(path,"r");
  if (fp == NULL){
    printf("the file pointer is NULL");
    char* errorchar="docID not found in crawledDir";
    return errorchar;
  }
  char* URL=freadlinep(fp);
  free(dircopy);
  fclose(fp);
  return URL;
}

/* Prints scores and URLS to stdout */
void printresults(struct page* matchesarray,int matches, char* crawlerdir){
  printf("Matches %d documents (ranked):\n",matches);
  for (int x=0;x<matches;x++){
    struct page resultpage=matchesarray[x];
    int docID=resultpage.docID;
    int score=resultpage.score;
    char* URL=getpageURL(docID,crawlerdir);
    printf("score  %d doc  %d: %s\n",score,docID,URL);
    free(URL);
  }
}

int main(int argc,char* argv[]){
  if (argc < 3 ){
    printf("need 2 arguments\n");
    return 1;
  }
  bool iscrawlerdir=isCrawlerDirectory(argv[1]);
  if (iscrawlerdir == false){
    return 1;
  }
  FILE* indexfile;
  indexfile=fopen(argv[2],"r");
  if (indexfile == NULL){
    printf("could not open %s for reading\n",argv[2]);
    return 1;
  }
  hashtable_t* index=loadindexfile(indexfile);
  if (argv[3] != NULL){
    if (strcmp(argv[3],"-q") == 0 && argv[4] != NULL){
      printf("reading from file\n");
    }
    FILE* queryfile;
    queryfile=fopen(argv[4],"r");
    if (queryfile == NULL){
      printf("could not open %s for reading\n",argv[4]);
      happly(index,deletewrapper);
      hclose(index);
      fclose(indexfile);
      return 1;
    }
    char* queryfromfile;
    while(!feof(queryfile)){
      queryfromfile=freadlinep(queryfile);
      if (queryfromfile == NULL){
        break;
      }
      if (queryfromfile[0] == '\0'){
        printf("empty line\n");
        free(queryfromfile);
        continue;
      }
      int arraysize=300;
      char* wordarray[arraysize];
      int array_counter=0;
      char* word=strtok(queryfromfile," ");
      char* fnormalized=NormalizeWord(word); 
      bool fisbadchar=badcharcheck(fnormalized);
      if (fisbadchar == true){
        free(queryfromfile);
        free(fnormalized);
        continue;
      }
      wordarray[array_counter]=fnormalized;
      bool isbadchar=false;
      while (word!=NULL){
        word=strtok(NULL, " ");
        if (word == NULL){
          break;
        }
        char* normalized=NormalizeWord(word);
        isbadchar=badcharcheck(normalized);
        if (isbadchar == true){
          free(queryfromfile);
          free(normalized);
          break;
        }
        if (array_counter >= arraysize-2){
          printf("too many arguments\n");
          return 1;
        }
        array_counter+=1;
        wordarray[array_counter]=normalized;
      }
      if (isbadchar == true){
        delete_wordarray(wordarray,array_counter);
        continue;
      }
      bool syntax=checksyntax(wordarray,array_counter);
      if (syntax == false){
        free(queryfromfile);
        delete_wordarray(wordarray,array_counter);
        continue;
      }
      printcleanquery(wordarray,array_counter);
      counters_t* result=processquery(wordarray,array_counter,index);
      int matches=0;
      int* matchesptr=&matches;
      counters_iterate(result,matchesptr,numofmatches);
      int index=0;
      int* indexptr=&index;
      struct page matchesarray[matches];
      struct resultarray args= {matchesarray,indexptr};
      counters_iterate(result,&args,&makeresultarray);
      sortmatchesarray(matchesarray,matches);
      if (matches == 0){
        printf("No documents match. \n");
        printf("-----------------------------------------------\n");
      }
      else{
        printresults(matchesarray,matches,argv[1]);
        printf("-----------------------------------------------\n");
      }
      counters_delete(result);
      free(queryfromfile);
      delete_wordarray(wordarray,array_counter);
    }
    fclose(queryfile);
  }
  char* query;
  while(!feof(stdin)){
    printf("query? ");
    query=freadlinep(stdin);
    if (query == NULL){
      break;
    }
    if (query[0] == '\0'){
      printf("empty line\n");
      free(query);
      continue;
    }
    int arraysize=300;
    char* wordarray[arraysize];
    int array_counter=0;
    char* word=strtok(query," ");
    char* fnormalized=NormalizeWord(word); 
    bool fisbadchar=badcharcheck(fnormalized);
    if (fisbadchar == true){
      free(query);
      free(fnormalized);
      continue;
    }
    wordarray[array_counter]=fnormalized;
    bool isbadchar=false;
    while (word!=NULL){
      word=strtok(NULL, " ");
      if (word == NULL){
        break;
      }
      char* normalized=NormalizeWord(word);
      isbadchar=badcharcheck(normalized);
      if (isbadchar == true){
        free(query);
        free(normalized);
        break;
      }
      if (array_counter >= arraysize-2){
        printf("too many arguments\n");
        return 1;
      }
      array_counter+=1;
      wordarray[array_counter]=normalized;
    }
    if (isbadchar == true){
      delete_wordarray(wordarray,array_counter);
      continue;
    }
    bool syntax=checksyntax(wordarray,array_counter);
    if (syntax == false){
      free(query);
      delete_wordarray(wordarray,array_counter);
      continue;
    }
    printcleanquery(wordarray,array_counter);
    counters_t* result=processquery(wordarray,array_counter,index);
    int matches=0;
    int* matchesptr=&matches;
    counters_iterate(result,matchesptr,numofmatches);
    int index=0;
    int* indexptr=&index;
    struct page matchesarray[matches];
    struct resultarray args= {matchesarray,indexptr};
    counters_iterate(result,&args,&makeresultarray);
    sortmatchesarray(matchesarray,matches);
    if (matches == 0){
      printf("No documents match. \n");
      printf("-----------------------------------------------\n");
    }
    else{
      printresults(matchesarray,matches,argv[1]);
      printf("-----------------------------------------------\n");
    }
    counters_delete(result);
    free(query);
    delete_wordarray(wordarray,array_counter);
  }
  happly(index,deletewrapper);
  hclose(index);
  fclose(indexfile);
  return 0;
}
