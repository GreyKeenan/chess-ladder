#include "./ladder.h"

#include "./player.h"

#include "utilities.h"
#include <string.h>


int ladder_findPlayers(
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
static inline void ladder_updateDate(struct ladder_player player[restrict static 1], int newDate);


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
			ladder_processDraw(&leader, date, winnerName, loserName);
			//sneeze("TODO: draw (#%d) ignored between \"%s\" and \"%s\".\n",
			//	matchCount, winnerName, loserName);
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
	int winnerHigher = ladder_findPlayers(*leader, winnerName, loserName, &winner, &loser);

	if (winnerHigher) {
		if (winner == NULL) panic("winner must exist\n");
		if (loser == NULL) {
			loser = ladder_player_new(loserName);
			ladder_player_append(*leader, loser);
		}
		winner->wins++;
		loser->losses++;
		ladder_updateDate(winner, date);
		ladder_updateDate(loser, date);
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
		ladder_updateDate(winner, date);
		ladder_updateDate(loser, date);
		return;
	}

	if (winner != NULL) ladder_player_pop(winner);
	else winner = ladder_player_new(winnerName);
	ladder_player_pass(winner, loser);
	winner->wins++;
	loser->losses++;
	if (loser == *leader) *leader = winner;
	ladder_updateDate(winner, date);
	ladder_updateDate(loser, date);
	return;
}

static inline void ladder_processDraw(
	struct ladder_player *leader[restrict static 1],
	unsigned int date,
	const char nameOne[restrict static ladder_STRMAX],
	const char nameTwo[restrict static ladder_STRMAX]
) {
	struct ladder_player *nick = NULL; // named in honor of the players who made the first draw in our club
	struct ladder_player *logan = NULL;
	int nickHigher = ladder_findPlayers(*leader, nameOne, nameTwo, &nick, &logan);

	/*
	Intended Functionality:
	Place the lower-player directly beneath the higher-player.

	if nickHigher:
		if logan exists:
			pop logan
		else:
			create logan
		place logan below nick
		increment draw counter
	else:
		if logan exists
			if nick exists:
				pop nick
			else:
				create nick
			place nick below logan
			increment draw counter
		else:
			(assert neither exist)
			create nick
			append nick
			create logan
			append logan
			increment draw counter
	*/

	if (nickHigher) {
		if (nick == NULL) panic("nick must exist.\n");

		if (logan == NULL) logan = ladder_player_new(nameTwo);
		else ladder_player_pop(logan);
		ladder_player_follow(nick, logan);
		nick->draws++;
		logan->draws++;
		ladder_updateDate(nick, date);
		ladder_updateDate(logan, date);
		return;
	}

	if (logan != NULL) {
		if (nick == NULL) nick = ladder_player_new(nameOne);
		else ladder_player_pop(nick);
		ladder_player_follow(logan, nick);
		nick->draws++;
		logan->draws++;
		ladder_updateDate(nick, date);
		ladder_updateDate(logan, date);
		return;
	}

	if (nick != NULL) panic("It must be that neither player exists here.\n");
	nick = ladder_player_new(nameOne);
	logan = ladder_player_new(nameTwo);
	ladder_player_append(*leader, nick); //arbitrary
	ladder_player_append(*leader, logan);
	nick->draws++;
	logan->draws++;
	ladder_updateDate(nick, date);
	ladder_updateDate(logan, date);

	return;
}


int ladder_findPlayers(
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

static inline void ladder_updateDate(struct ladder_player player[restrict static 1], int newDate)
{
	if (newDate > player->mostRecentGame) player->mostRecentGame = newDate;
}
