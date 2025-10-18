#ifndef MAIN_player
#define MAIN_player


#include "./input.h"


struct player {
	struct player *nigh[2];
	char name[MAXSTRING];
	int wincount;
	int drawcount;
	int losscount;
};


struct player * /*nonull*/ player_new(const char * /*nonull*/ name);

int player_pop(struct player * /*nonull*/ self);
int player_append(struct player * /*nonull*/ head, struct player * /*nonull*/ self);
int player_insertAbove(struct player * /*nonull*/ aboveMe, struct player * /*nonull*/ insertMe);


#endif
