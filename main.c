#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./utilities.h"
#include "./player.h"
#include "./input.h"
#include "./output.h"


#define MATCHHISTORYFILE "input.csv"


int climb(struct player * /*nonull*/ head, int isDraw, const char winnerName[restrict static MAXSTRING], const char loserName[restrict static MAXSTRING]);
int findPlayers(
	struct player * /*nonull*/ head,
	const char winner[restrict static MAXSTRING],
	const char loser[restrict static MAXSTRING],
	struct player *winner_out[restrict static 1],
	struct player *loser_out[restrict static 1]
);


int main(int argc, char **argv)
{

	struct player *head = player_new("");

	FILE *f = fopen(MATCHHISTORYFILE, "r");
	if (f == NULL) panic("failed to open file\n");

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

		if (winner == NULL) panic("winner must exist because they were found first\n");

		if (loser == NULL) {
			loser = player_new(loserName);
			player_append(head, loser);
		}

		winner->wincount++;
		loser->losscount++;

		return 0;
	}

	/* untrue, might be that neither exist
	if (loser == NULL) panic("loser must exist because they were found first\n");
	*/
	if (loser == NULL) {
		loser = player_new(loserName);

		if (winner != NULL) panic("winner must not exist either, otherwise would be found first\n");

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

