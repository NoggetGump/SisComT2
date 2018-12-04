#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pages.h"
//#include "listas.h" definido em pages.h

#define KBYTES_BYTES 1000
#define MBYTES_BYTES 1000000

unsigned int setRightShift(unsigned int maxBytes){
	unsigned int binSize = 2;
	unsigned int shift = 1;

	if(maxBytes == 0)
		return 0;
	while(binSize - 1 < maxBytes){
		binSize *= 2;
//		printf("\nbinsize - %u\n", binSize);
		shift++;
	}

	return shift;
}

int main(int argc, char* argv[]){
	List* pages;
//	Page* currentPage;
	FILE* file = fopen(argv[2], "r");
	FILE* answrFile = fopen("answr.txt", "w");
	unsigned int fileSize = atoi(argv[3]);
	unsigned int memorySize = atoi(argv[4]);
	unsigned int shift, addr, listSize, maxPages, writenPages = 0, pageFaults = 0, timer = 0;
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

	printf("\nSomething - %u\n", fileSize*KBYTES_BYTES);
	shift  = setRightShift(fileSize*KBYTES_BYTES);
	printf("\nshift - %u\n", shift);
	maxPages = (memorySize*MBYTES_BYTES)/(fileSize*KBYTES_BYTES);
	printf("\nExecutando...\n");
	fprintf(answrFile, "--- EXECUTION LOG ---\n");
	while(fscanf(file, "%x %c ", &addr, &rw) == 2) {
		Page* page;
		Page* temp;
		unsigned int key = addr >> shift;

	//	printf("\n%x\n", key);
		createPage(&page);
		setAddr(page, key);

		if(rw == 'R'){
			setReferenced(page, 1);
			setModified(page, 0);
		}
		else{
			setReferenced(page, 0);
			setModified(page, 1);
		}
		if(timer == 100){
			timer = 0;
			cleaRM(pages);
		}
		list_size(pages, &listSize);
		if(listSize < maxPages) {
			if((temp = search4key(pages, getAddr(page))) == NULL) {
				push_back(pages, page);
				fprintf(answrFile, "\nAddress - %u - Page Fault\n", getAddr(page));
				fprintf(answrFile, "Referenced - %d\n", getReferenced(page));
				fprintf(answrFile, "Modified - %d\n", getModified(page));
				fprintf(answrFile, "__________________________\n");
				writenPages++;
				pageFaults++;
			}
			else {
				modifyRM(temp, getReferenced(page));
				fprintf(answrFile, "\nAddress - %u - Succesfully Read or Modified\n", getAddr(temp));
				fprintf(answrFile, "Referenced - %d\n", getReferenced(temp));
				fprintf(answrFile, "Modified - %d\n", getModified(temp));
				fprintf(answrFile, "__________________________\n");
				free(page);
			}
		}
		else {
			if((temp = search4key(pages, getAddr(page))) == NULL) {
				//algoritmo de substituição entra aqui
				temp = search4leastUsed(pages);
				temp = page;
				fprintf(answrFile, "\nAddress - %u - Page Fault\n", getAddr(page));
				fprintf(answrFile, "Referenced - %d\n", getReferenced(page));
				fprintf(answrFile, "Modified - %d\n", getModified(page));
				fprintf(answrFile, "__________________________\n");
				pageFaults++;
			}
			else {
				modifyRM(temp, getReferenced(page));
				fprintf(answrFile, "\nAddress - %u - Succesfully Read or Modified\n", getAddr(temp));
				fprintf(answrFile, "Referenced - %d\n", getReferenced(temp));
				fprintf(answrFile, "Modified - %d\n", getModified(temp));
				fprintf(answrFile, "__________________________\n");
				free(page);
			}
		}
		timer ++;
	}

	printf("\nAlgoritmo de substituicao = %s\n", argv[1]);
	printf("\nArquivo de entrada = %s\n", argv[2]);
	printf("\nFile size = %u KB\n", fileSize);
	printf("\nMemory size = %u MB\n", memorySize);
	printf("\nPage Faults = %u\n", pageFaults);
	printf("\nPaginas Escritas = %u\n\n", writenPages);

	return 0;
}