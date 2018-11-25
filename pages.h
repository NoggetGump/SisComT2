#include "listas.h"

#ifndef PAGES_H
#define PAGES_H

typedef struct page Page;

void createPage(Page** page);

void setAddr(Page* page, unsigned int addr);

void setReferenced(Page* page, short R);

void setModified(Page* page, short M);

unsigned int getAddr(Page* page);

short getReferenced(Page* page);

short getModified(Page* page);

void modifyRM(Page* page, short R);

void cleaRM(List* pages);

Page* search4key(List* pages, unsigned int key);

Page* search4leastUsed(List* pages);

#endif