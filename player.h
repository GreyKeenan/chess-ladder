#ifndef LADDER_player
#define LADDER_player


#include "./ladder.h"

struct ladder_player {
	struct ladder_player /*nonull*/ *superior;
	struct ladder_player /*nonull*/ *inferior;

	char name[ladder_STRMAX];
	unsigned int mostRecentGame;
	unsigned int wins;
	unsigned int draws;
	unsigned int losses;
};
/*
The ladder is represented using a doubly-linked list.
"superior" points to the player immediately above this one in the ladder.
"inferior" points to the player immediately below.
The number-one and last-place players point to each other.

Rather than being NULL, superior & inferior should point to itself,
creating a doubly-linked-list with 1 entry.
*/

struct ladder_player * /*nonull*/ ladder_player_new(const char * /*nonull*/ name);
/*
Creates a new player on the heap.

"name" is a null-terminated string.
If the length of exceeds STRMAX,
it will be truncated and the null-byte will be added to the end.
*/

void ladder_player_pop(struct ladder_player * /*nonull*/ self);
/*
Pops a player from the linked list.
Has no effect if it already is in a linked-list of 1 item.
*/

void ladder_player_pass(struct ladder_player * /*nonull*/ winner, struct ladder_player * /*nonull*/ loser);
/*
Places the winner as the loser's superior in the linked list.
Assumes that the winner is not already in a linked list.
*/
static inline void ladder_player_append(struct ladder_player * /*nonull*/ numberOne, struct ladder_player * /*nonull*/ appendMe)
{
	ladder_player_pass(appendMe, numberOne);
}

void ladder_player_follow(struct ladder_player * /*nonull*/ ahead, struct ladder_player * /*nonull*/ behind);
/*
Places "behind" as "ahead"'s inferior.
Assumes that "behind" is not already in a linked list.
*/


#endif
