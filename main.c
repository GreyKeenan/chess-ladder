#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "./utilities.h"
#include "ladder.h"
#include "player.h"


typedef int ladder_player_ASSERT_INTSIZE[(sizeof(int) > 2)? 1:-1];
// confirm that the ladder_player.mostRecentGame field can hold arbitrary 6 digits
// throws a compile-time error if int size is too small


void printLadder(struct ladder_player *leader);
void printLadder_csv(struct ladder_player *leader);
ladder_matchSource readMatch;
int readUntil(FILE *f, char *output, int maxLength, char delimiter);


int main(int argc, char **argv)
{
	if (argc != 2) panic("This program requires exactly 1 command-line argument. Please provide the name of an appropriately-formatted CSV file to read from.\n");
	char *inputFile = argv[1];

	FILE *f = fopen(inputFile, "r");
	if (f == NULL) panic("failed to open file: %s\n", inputFile);

	struct ladder_player *leader = NULL;

	int e = ladder_generate(&leader, f, &readMatch);
	if (e) panic("ladder generation failed with code (%d)\n", e);

	//printLadder(leader);
	printLadder_csv(leader);

	fclose(f);
	// don't need to bother freeing the linked list in this case
	return 0;
}

void printLadder_csv(struct ladder_player *leader)
{
	if (leader == NULL) {
		printf("NULL LADDER\n");
		return;
	}

	printf("Position,Name,Wins,Losses,Draws,TotalGames,LastPlayed\n");

	unsigned int i = 0;
	struct ladder_player *current = leader;
	do {
		if (current == NULL) panic("NULL entry in the leaderboard\n");

		printf("%u,%s,%d,%d,%d,%d,%d\n",
			++i,
			current->name,
			current->wins,
			current->losses,
			current->draws,
			current->wins + current->losses + current->draws,
			current->mostRecentGame
		);

		//printf("[%u][%dw %dl]\t %s", ++i, current->wins, current->losses, current->name);
		//printf(" (%d)\n", current->mostRecentGame);
		current = current->inferior;
	} while (current != leader);
	printf("\n");
}

void printLadder(struct ladder_player *leader)
{
	printf("\nChess Ladder:\n====================\n\n");

	if (leader == NULL) {
		printf("0 players\n");
		return;
	}

	unsigned int i = 0;
	struct ladder_player *current = leader;
	do {
		if (current == NULL) panic("NULL entry in the leaderboard\n");
		printf("[%u][%dw %dl]\t %s", ++i, current->wins, current->losses, current->name);
		printf(" (%d)\n", current->mostRecentGame);
		current = current->inferior;
	} while (current != leader);

	printf("\n");
}

ladder_matchSource readMatch;
int readMatch(
	void *f, // is a FILE pointer
	unsigned int date[restrict static 1],
	int isDraw[restrict static 1],
	char winner[restrict static ladder_STRMAX],
	char loser[restrict static ladder_STRMAX]
) {

	int bytesRead = 0;

	bytesRead = readUntil(f, NULL, 0, ','); // ID
	if (bytesRead < 0) return -1;
	bytesRead = readUntil(f, winner, ladder_STRMAX, ','); // winner
	if (bytesRead < 0) return -1;
	bytesRead = readUntil(f, loser, ladder_STRMAX, ','); // loser
	if (bytesRead < 0) return -1;

	bytesRead = readUntil(f, NULL, 0, ','); // drawFlag
	if (bytesRead < 0) return -1;
	*isDraw = (bytesRead > 1); // means it read some data besides just the comma

	char dateString[ladder_STRMAX] = {0};
	bytesRead = readUntil(f, dateString, ladder_STRMAX, '\n'); // date
	if (bytesRead < 0) return -1;
	// TODO validate string
	*date = atoi(dateString);

	return 0;
}

int readUntil(FILE *f, char *output, int maxLength, char delimiter)
{
	int c = 0;
	int bytesRead = 0;
	int bytesWritten = 0;
	while (1) {
		if (bytesRead == INT_MAX) panic("read too far before delimiter reached\n");
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
