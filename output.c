#include "./output.h"


int printLadder(struct player * /*nonull*/ head)
{

	printf("Chess Ladder:\n=========================\n\n");

	// printf("[-1] %s\n", head->nigh[0]->name); //DEBUG

	unsigned int i = 0;
	struct player * /*nonull*/ current = head;
	while (1) {
		if (current->nigh[1] == NULL || current->nigh[1] == head) break;
		current = current->nigh[1];
		//if (current->wincount == 0) continue;
		printf("[%u][%dw %dl]\t %s\n", ++i, current->wincount, current->losscount, current->name);
	}
	return 0;
}

