#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pages.h"

struct page{
	unsigned int addr;
	short R;
	short M;
};

unsigned int setRightShift(unsigned int size){
	unsigned int binSize = 2;
	unsigned int shift = 0;

	while(binSize < size){
		binSize *= 2;
		shift++;
	}

	return 32 - shift;
}

void modifyRM(Page* page, short R){
	if(R == 1)
		page->R = 1;
	else
		page->M = 1;
}

Page* search4key(List* pages, unsigned int key){
	Page* currentPage;

	first(pages);
	if(get_val_cursor(pages, (void**) &currentPage) != LIS_CondRetOK)
		return NULL;
	while(currentPage->addr != key)
	{
		if(next(pages) != LIS_CondRetOK)
			return NULL;
		get_val_cursor(pages, (void**) &currentPage);
	}

	return currentPage;
}

Page* search4leastUsed(List* pages){
	Page* currentPage;

	first(pages);
	if(get_val_cursor(pages, (void**) &currentPage) != LIS_CondRetOK)
		return NULL;
	do {
		if(currentPage->R == 0 && currentPage->M == 0)
			return currentPage;
	} while(next(pages) == LIS_CondRetOK);

	first(pages);
	do {
		if(currentPage->R == 0 && currentPage->M == 1)
			return currentPage;
	} while(next(pages)  == LIS_CondRetOK);

	first(pages);
	do {
		if(currentPage->R == 1 && currentPage->M == 0)
			return currentPage;
	} while(next(pages) == LIS_CondRetOK);

	first(pages);
	get_val_cursor(pages, (void**) &currentPage);

	return currentPage;
}