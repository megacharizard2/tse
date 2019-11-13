/* locked-hash.h --- locked hash interface
 * 
 * 
 * Author: Samuel H. Baker
 * Created: Wed Nov  6 17:05:04 2019 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef void lhash_t;

lhash_t *lhopen(uint32_t hsize);

int32_t lhput(lhash_t *lhtp, void *ep, const char *key, int keylen);

void lhapply(lhash_t *lhtp, void (*fn)(void* ep));

void* lhsearch(lhash_t *lhtp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen);

void* lhremove(lhash_t* lhtp,
							 bool (*searchfn)(void* elementp, const void* searchkeyp),
							 const char *key,
							 int32_t keylen);

void lhclose(lhash_t* lhtp);
