#pragma once

#include <stdio.h>
#include <stdbool.h>

typedef struct counters counters_t;
typedef struct wrapper wrapper_t;

void *
assertp(void *p, const char *message);

wrapper_t* wrapper_new(char* word,counters_t* ctrs);

char* wrapper_getword(wrapper_t* wrapper);

counters_t* wrapper_getcounter(wrapper_t* wrapper);

void wrapper_delete(wrapper_t* wrapper);

counters_t *counters_new(void);

void counters_add(counters_t *ctrs, const int key);

int counters_get(counters_t *ctrs, const int key);

void counters_set(counters_t *ctrs, const int key, int count);

void counters_print(counters_t *ctrs, FILE *fp);

void counters_iterate(counters_t *ctrs, void *arg,
                      void (*itemfunc)(void *arg, const int key, int count));

void counters_delete(counters_t *ctrs);

extern char *freadlinep(FILE *fp);
static inline char *readlinep(void) { return freadlinep(stdin); }

char *readrest(FILE *fp);
