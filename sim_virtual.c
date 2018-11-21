#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pages.h"
//#include "listas.h" definido em pages.h

#define KBYTES_BITS 8000
#define KBYTES_BYTES 1000
#define MBYTES_BYTES 1000000

int main(int argc, char* argv[]){
	List* pages;
	Page* currentPage;
	FILE* file = fopen(argv[1], "r");
	unsigned int fileSize = atoi(argv[2]);
	unsigned int memorySize = atoi(argv[3]);
	unsigned int shift, addr, listSize, maxPages;
	char rw;

	createList(&pages);

	if(fileSize < 8 || fileSize > 32){
		printf("\nTamanho de arquivo incorreto,\n selecione um valor entre 8 e 32 (KB).\n\n");
		return 0;
	}
	if(memorySize < 1 || memorySize > 16){
		printf("\nTamanho de memória física incorreto,\n selecione um valor entre 1 e 16 (MB).\n\n");
		return 0;
	}

	shift  = setRightShift(fileSize*KBYTES_BITS);
	maxPages = memorySize*MBYTES_BYTES/fileSize*KBYTES_BYTES;

	while(fscanf(file, "%x %c ", &addr, &rw) == 2) {
		Page* page = (Page*) malloc(sizeof(Page));
		Page* temp;
		unsigned int key = addr >> shift;

		page->addr = key;

		if(rw == 'R'){

			page->R = 1;
			page->M = 0;
		}	
		else{
			page->R = 0;
			page->M = 1;
		}

		if(list_size(pages, &listSize) < maxPages) {
			if((temp = search4key(pages, page->addr)) == NULL)
				push_back(pages, page);
			else {
				modifyRM(temp, page->R);
				free(page);
			}
		}
		else {
			if((temp = search4key(pages, page->addr)) == NULL) {
				printf("\nAlgoritmo de selecao entra aqui.\n");
			}
			else {
				modifyRM(temp, page->R);
				free(page);
			}
		}
	}
	first(pages);
	get_val_cursor(pages, (void**) &currentPage);
	printf("\n%x\n", currentPage->addr);
	printf("\n%c\n", rw);
	printf("\n%hd", currentPage->R);
	printf("%hd\n", currentPage->M);
	while(next(pages) == LIS_CondRetOK) {
		get_val_cursor(pages, (void**) &currentPage);
		printf("\n%x\n", currentPage->addr);
		printf("\n%c\n", rw);
		printf("\n%hd", currentPage->R);
		printf("%hd\n", currentPage->M);
	}

	return 0;
}