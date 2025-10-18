#include "./ladder.h"

#include "./player.h"

#include "utilities.h"
#include <string.h>


int findPlayers(
	struct ladder_player * /*nonull*/ leader,
	const char winnerName[restrict static ladder_STRMAX],
	const char loserName[restrict static ladder_STRMAX],
	struct ladder_player * winner[restrict static 1],
	struct ladder_player * loser[restrict static 1]
);
static inline void ladder_processVictory(
	struct ladder_player *leader[restrict static 1],
	unsigned int date,
	const char winnerName[restrict static ladder_STRMAX],
	const char loserName[restrict static ladder_STRMAX]
);
static inline void ladder_processDraw(
	struct ladder_player *leader[restrict static 1],
	unsigned int date,
	const char oneName[restrict static ladder_STRMAX],
	const char twoName[restrict static ladder_STRMAX]
);


int ladder_generate(
	struct ladder_player *output[restrict static 1],
	void *matchesContext, ladder_matchSource * /*nonull*/ matches
) {

	int e = 0;

	unsigned int matchCount = 0;

	unsigned int date = 0;
	int isDraw = 0;
	char winnerName[ladder_STRMAX] = {0};
	char loserName[ladder_STRMAX] = {0};

	struct ladder_player * leader = NULL;

	while (1) {
		e = matches(matchesContext, &date, &isDraw, winnerName, loserName);
		if (e) break;
		matchCount++;

		//sneeze("match #%d: (%d)(%d)(%s)(%s)\n", matchCount, date, isDraw, winnerName, loserName);

		if (isDraw) {
			//ladder_processDraw(&leader, date, winnerName, loserName);
			sneeze("TODO: draw (#%d) ignored between \"%s\" and \"%s\".\n",
				matchCount, winnerName, loserName);
			continue;
		}
		ladder_processVictory(&leader, date, winnerName, loserName);
	}

	*output = leader;
	return (e > 0)? e:0;
}

static inline void ladder_processVictory(
	struct ladder_player *leader[restrict static 1],
	unsigned int date,
	const char winnerName[restrict static ladder_STRMAX],
	const char loserName[restrict static ladder_STRMAX]
) {

	struct ladder_player *winner = NULL;
	struct ladder_player *loser = NULL;
	int winnerHigher = findPlayers(*leader, winnerName, loserName, &winner, &loser);

	if (winnerHigher) {
		if (winner == NULL) panic("winner must exist\n");
		if (loser == NULL) {
			loser = ladder_player_new(loserName);
			ladder_player_append(*leader, loser);
		}
		winner->wins++;
		loser->losses++;
		//TODO: update date
		return;
	}

	if (loser == NULL) {
		if (winner != NULL) panic("winner shouldn't exist, otherwise would be higher\n");

		winner = ladder_player_new(winnerName);
		loser = ladder_player_new(loserName);

		if (*leader == NULL) *leader = winner;
		else ladder_player_append(*leader, winner);
		ladder_player_append(*leader, loser);

		winner->wins++;
		loser->losses++;
		//TODO: update date
		return;
	}

	if (winner != NULL) ladder_player_pop(winner);
	else winner = ladder_player_new(winnerName);
	ladder_player_pass(winner, loser);
	winner->wins++;
	loser->losses++;
	if (loser == *leader) *leader = winner;
	//TODO: update date
	return;
}

static inline void ladder_processDraw(
	struct ladder_player *leader[restrict static 1],
	unsigned int date,
	const char oneName[restrict static ladder_STRMAX],
	const char twoName[restrict static ladder_STRMAX]
) {
	struct ladder_player *one = NULL;
	struct ladder_player *two = NULL;
	int oneHigher = findPlayers(*leader, oneName, twoName, &one, &two);

	// TODO
}


int findPlayers(
	struct ladder_player * leader,
	const char winnerName[restrict static ladder_STRMAX],
	const char loserName[restrict static ladder_STRMAX],
	struct ladder_player * winner[restrict static 1],
	struct ladder_player * loser[restrict static 1]
) {

	*winner = NULL;
	*loser = NULL;

	if (leader == NULL) return 0;

	struct ladder_player * /*nonull*/ current = leader;
	int winnerFoundFirst = 0;
	do {

		//sneeze("[%p] \"%s\"\n", (void*)current, current->name);

		if (!strncmp(current->name, winnerName, ladder_STRMAX)) {
			if (*loser == NULL) winnerFoundFirst = 1;
			if (*winner != NULL) panic("winner was found twice\n");
			*winner = current;
		} else if (!strncmp(current->name, loserName, ladder_STRMAX)) {
			if (*loser != NULL) panic("loser was found twice\n");
			*loser = current;
		}

		current = current->inferior;
	} while (current != leader);

	return winnerFoundFirst;
}
