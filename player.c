#include "./player.h"

#include "utilities.h"

#include <string.h>


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

