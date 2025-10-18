#include "./player.h"

#include "./utilities.h"

#include <string.h>


struct ladder_player * /*nonull*/ ladder_player_new(const char * /*nonull*/ name)
{
	struct ladder_player *self = malloc(sizeof(struct ladder_player));
	if (self == NULL) panic("malloc failed\n");

	*self = (struct ladder_player){0};

	self->superior = self;
	self->inferior = self;

	strncpy(self->name, name, ladder_STRMAX);
	self->name[ladder_STRMAX - 1] = '\0';

	return self;
}

void ladder_player_pop(struct ladder_player * /*nonull*/ self)
{
	self->superior->inferior = self->inferior;
	self->inferior->superior = self->superior;
	self->superior = self;
	self->inferior = self;
}
void ladder_player_pass(struct ladder_player * /*nonull*/ winner, struct ladder_player * /*nonull*/ loser)
{
	winner->inferior = loser;
	winner->superior = loser->superior;
	loser->superior->inferior = winner;
	loser->superior = winner;
}
void ladder_player_follow(struct ladder_player * /*nonull*/ ahead, struct ladder_player * /*nonull*/ behind)
{
	behind->superior = ahead;
	behind->inferior = ahead->inferior;
	ahead->inferior->superior = behind;
	ahead->inferior = behind;
}
