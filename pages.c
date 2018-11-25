#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pages.h"

struct page {
	unsigned int addr;
	short R;
	short M;
};

void createPage(Page** page){
	(*page) = (Page*) malloc(sizeof(Page));
	(*page)->addr = 0;
	(*page)->R = 0;
	(*page)->M = 0;
}

void setAddr(Page* page, unsigned int addr){
	page->addr = addr;
}

void setReferenced(Page* page, short R){
	page->R = R;
}

void setModified(Page* page, short M){
	page->M = M;
}

unsigned int getAddr(Page* page){
	return page->addr;
}

short getReferenced(Page* page){
	return page->R;
}

short getModified(Page* page){
	return page->M;
}

void modifyRM(Page* page, short R){
	if(R == 1)
		page->R = 1;
	else
		page->M = 1;
}

void cleaRM(List* pages) {
	Page* currentPage;

	first(pages);
	if(get_val_cursor(pages, (void**) &currentPage) != LIS_CondRetOK)
		return;
	currentPage->R = 0;
	currentPage->M = 0;
	do {
		get_val_cursor(pages, (void**) &currentPage);
		currentPage->R = 0;
		currentPage->M = 0;
	} while(next(pages) == LIS_CondRetOK);
}

Page* search4key(List* pages, unsigned int key){
	Page* currentPage;

	first(pages);
	if(get_val_cursor(pages, (void**) &currentPage) != LIS_CondRetOK)
		return NULL;
	while(currentPage->addr != key)	{
		if(next(pages) != LIS_CondRetOK)
			return NULL;
		get_val_cursor(pages, (void**) &currentPage);
	}

	return currentPage;
}

Page* search4leastUsed(List* pages){
	Page* currentPage;

	first(pages);
	if(get_val_cursor(pages, (void**) &currentPage) != LIS_CondRetOK) {
		printf("LIST ERROR IN EXECUTION");
		return NULL;
	}
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