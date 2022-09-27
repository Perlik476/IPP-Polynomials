/** @file
  Implementacja funkcji zarządzających pamięcią

  @author Mateusz Perlik <mp429581@students.mimuw.edu.pl>
  @date 2021
*/

#include "memory.h"


void *SafeCalloc(size_t num, size_t size) {
  if (size == 0) {
    return NULL;
  }

  void *ptr = calloc(num, size);
  if (ptr == NULL) {
    exit(1);
  }

  return ptr;
}


void *SafeMalloc(size_t size) {
  if (size == 0) {
    return NULL;
  }

  void *ptr = malloc(size);
  if (ptr == NULL) {
    exit(1);
  }

  return ptr;
}


void *SafeRealloc(void *ptr, size_t size) {
  if (ptr == NULL) {
    return SafeMalloc(size);
  }

  if (size == 0) {
    return NULL;
  }

  ptr = realloc(ptr, size);
  if (ptr == NULL) {
    exit(1);
  }

  return ptr;
}