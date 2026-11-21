#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

//
// The macros are used to store int values in a void* pointer.
//
#define PTR_FROM_INT(val) ((void *)(intptr_t)(val))
#define PTR_TO_INT(ptr) ((int)(intptr_t)(ptr))

#endif