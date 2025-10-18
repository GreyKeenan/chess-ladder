#ifndef LADDER
#define LADDER


#define ladder_STRMAX 32


struct ladder_player;


typedef int ladder_matchSource(void *context,
	unsigned int date[restrict static 1],
	int isDraw[restrict static 1],
	char winner[restrict static ladder_STRMAX],
	char loser[restrict static ladder_STRMAX]
);
/*
An fptr-style interface used by ladder_generate().
Each subsequent call will provide the data for the next match to process.

Character-arrays will still be null-terminated strings.

If isDraw is true, it doesn't matter which player is the winner/loser.

"date" will be in any form where a later-date is represented by a larger-number.

returns 0 on success
returns a negative value if called when there are no more matches to provide
returns a positive value on error
*/

int ladder_generate(
	struct ladder_player *output[restrict static 1],
	void *matchesContext, ladder_matchSource * /*nonull*/ matches
);
/*
Generates the chess ladder from the given match history.
The resulting value of "output" will be a pointer to the #1 player.
The leaderboard is represented as a linked list in the heap,
and it is the responsibility of the caller to free each node when done with it.

matchesContext will be passed as the first argument for calls to matches().

returns 0 on success
A positive return value is one that has been passed out from an inner matches() call.
*/


#endif
