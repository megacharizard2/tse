#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "webpage.h"
#include "hash.h"
#include "tools.h"
#include "queue.h"
#include <ctype.h>

char* NormalizeWord(char* word);

int loadcrawlerdir(char* dirname,int docid,hashtable_t* ht);

hashtable_t* loadindexfile(FILE* fp);




