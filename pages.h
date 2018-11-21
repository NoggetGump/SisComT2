#include "listas.h"

#ifndef PAGES_H
#define PAGES_H

typedef struct page Page;

unsigned int setRightShift(unsigned int size);

void modifyRM(Page* page, short R);

Page* search4key(List* pages, unsigned int key);

Page* search4leastUsed(List* pages);

#endif