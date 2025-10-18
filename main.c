#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>


#define MATCHHISTORYFILE "input.csv"


#define MAXSTRING 32
struct player {
	struct player *nigh[2];
	char name[MAXSTRING];
	int wincount;
	int drawcount;
	int losscount;
};


int readUntil(FILE *f, char *output, int maxLength, char delimiter);
int printLadder(struct player * /*nonull*/ head);
int climb(struct player * /*nonull*/ head, int isDraw, const char winnerName[restrict static MAXSTRING], const char loserName[restrict static MAXSTRING]);
int findPlayers(
	struct player * /*nonull*/ head,
	const char winner[restrict static MAXSTRING],
	const char loser[restrict static MAXSTRING],
	struct player *winner_out[restrict static 1],
	struct player *loser_out[restrict static 1]
);


struct player * /*nonull*/ player_new(const char * /*nonull*/ name)
{
	struct player *self = malloc(sizeof(struct player));
	if (self == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(1);
	}
	*self = (struct player){0};
	strncpy(self->name, name, MAXSTRING);
	self->name[MAXSTRING - 1] = '\0';
	return self;
}
int player_pop(struct player * /*nonull*/ self)
{

	if (self->nigh[0] == NULL) {
		fprintf(stderr, "left-neighbor should never be NULL for a node that is in the list presently\n");
		exit(1);
	}

	self->nigh[0]->nigh[1] = self->nigh[1];

	if (self->nigh[1] == NULL) {
		fprintf(stderr, "right-neighbor should never be NULL for a node that is in the list presently\n");
		exit(1);
	}

	self->nigh[1]->nigh[0] = self->nigh[0];

	self->nigh[1] = NULL;
	self->nigh[0] = NULL;

	return 0;
}
int player_append(struct player * /*nonull*/ head, struct player * /*nonull*/ self)
{
	self->nigh[1] = head;
	if (head->nigh[0] == NULL) {
		head->nigh[0] = self;
		head->nigh[1] = self;
		self->nigh[0] = head;
		return 0;
	}

	head->nigh[0]->nigh[1] = self;
	self->nigh[0] = head->nigh[0];
	head->nigh[0] = self;

	return 0;
}
int player_insertAbove(struct player * /*nonull*/ aboveMe, struct player * /*nonull*/ insertMe)
{

	if (aboveMe->nigh[0] == NULL) {
		fprintf(stderr, "should never be NULL since head is above all\n");
		exit(1);
	}

	aboveMe->nigh[0]->nigh[1] = insertMe;
	insertMe->nigh[0] = aboveMe->nigh[0];
	aboveMe->nigh[0] = insertMe;
	insertMe->nigh[1] = aboveMe;

	return 0;
}


int main(int argc, char **argv)
{

	struct player *head = player_new("");

	FILE *f = fopen(MATCHHISTORYFILE, "r");
	if (f == NULL) {
		fprintf(stderr, "failed to open file\n");
		exit(1);
	}

	int bytesRead = 0;
	char idfield[MAXSTRING] = {0};
	char winner[MAXSTRING] = {0};
	char loser[MAXSTRING] = {0};
	int isDraw = 0;
	//char TEMP;
	//int TEMP2;
	while (1) {

		bytesRead = readUntil(f, idfield, MAXSTRING, ','); // ID
		if (bytesRead < 0) break;
		bytesRead = readUntil(f, winner, MAXSTRING, ','); // winner
		if (bytesRead < 0) break;
		bytesRead = readUntil(f, loser, MAXSTRING, ','); // loser
		if (bytesRead < 0) break;

		bytesRead = readUntil(f, NULL, 0, ','); // drawFlag
		if (bytesRead < 0) break;
		isDraw = (bytesRead > 1); // means it read some data besides just the comma

		bytesRead = readUntil(f, NULL, 0, '\n'); // date

		if (isDraw) {
			isDraw = 0;
			printf("TODO: draw (#%s) ignored between \"%s\" and \"%s\".\n",
				idfield, winner, loser);
			continue;
		}

		climb(head, 0, winner, loser);
		if (bytesRead < 0) break;

		/* //DEBUG
		system("clear");
		printf("(%d) ", TEMP2);
		TEMP2++;
		printLadder(head);
		//printf("\n\n");
		scanf("%c", &TEMP);
		*/

	}

	fclose(f);

	printf("\n\n");
	printLadder(head);

	// free LL if prog didn't end here

	return 0;
}

int readUntil(FILE *f, char *output, int maxLength, char delimiter)
{
	int c = 0;
	int bytesRead = 0;
	int bytesWritten = 0;
	while (1) {
		if (bytesRead == INT_MAX) {
			fprintf(stderr, "read too far before delimiter reached\n");
			exit(1);
		}
		c = getc(f);
		bytesRead++;

		if (c == delimiter) break;
		else if (c == EOF) {
			bytesRead *= -1;
			break;
		}

		if (bytesRead <= maxLength) {
			output[bytesRead - 1] = c;
			bytesWritten++;
		}
	}

	if (output == NULL || maxLength == 0) return bytesRead;

	if (bytesWritten >= maxLength) {
		output[maxLength - 1] = '\0';
	} else {
		output[bytesWritten] = '\0';
	}

	return bytesRead;
}

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

int climb(struct player * /*nonull*/ head, int isDraw, const char winnerName[restrict static MAXSTRING], const char loserName[restrict static MAXSTRING])
{

	struct player * winner = NULL;
	struct player * loser = NULL;
	int winnerAlreadyHigher = findPlayers(head, winnerName, loserName, &winner, &loser);

	if (isDraw) {
		if (winner == NULL) {
			winner = player_new(winnerName);
			player_append(head, winner);
		}
		if (loser == NULL) {
			loser = player_new(loserName);
			player_append(head, loser);
		}
		winner->drawcount++;
		loser->drawcount++;
		return 0;
	}

	if (winnerAlreadyHigher) {

		if (winner == NULL) {
			fprintf(stderr, "winner must exist because they were found first\n");
			exit(1);
		}

		if (loser == NULL) {
			loser = player_new(loserName);
			player_append(head, loser);
		}

		winner->wincount++;
		loser->losscount++;

		return 0;
	}

	/* untrue, might be that neither exist
	if (loser == NULL) {
		fprintf(stderr, "loser must exist because they were found first\n");
		exit(1);
	}
	*/
	if (loser == NULL) {
		loser = player_new(loserName);

		if (winner != NULL) {
			fprintf(stderr, "winner must not exist either, otherwise would be found first\n");
			exit(1);
		}

		winner = player_new(winnerName);

		player_append(head, winner);
		player_append(head, loser);
		winner->wincount++;
		loser->losscount++;
		return 0;
	}

	if (winner == NULL) {
		winner = player_new(winnerName);
	} else {
		player_pop(winner);
	}

	player_insertAbove(loser, winner);
	winner->wincount++;
	loser->losscount++;

	return 1; // `1` indicates that there was a change to the ladder rankings
}

int findPlayers(
	struct player * /*nonull*/ head,
	const char winner[restrict static MAXSTRING],
	const char loser[restrict static MAXSTRING],
	struct player *winner_out[restrict static 1],
	struct player *loser_out[restrict static 1]
)
{
	struct player * /*nonull*/ current = head;
	*winner_out = NULL;
	*loser_out = NULL;
	int winnerWasFoundFirst = 0;
	while (1) {

		if (current->nigh[1] == NULL || current->nigh[1] == head) break;
		current = current->nigh[1];

		if (!strncmp(current->name, winner, MAXSTRING)) {
			if (*loser_out == NULL) winnerWasFoundFirst = 1;
			*winner_out = current;
		} else if (!strncmp(current->name, loser, MAXSTRING)) {
			*loser_out = current;
		}

	}

	return winnerWasFoundFirst;
}

