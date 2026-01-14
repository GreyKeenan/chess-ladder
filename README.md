
This is a quick, thrown-together utility to automate
our chess-ladder for the Normandale Chess Club.


### Todo

* input validation is important


A Tiny Bit of Documentation
========================================

Here is the minimum amount of information.
This is all you get, sorry.


Using the program
----------------------------------------

### Input

From a record of wins/losses/draws,
the program generates a ladder-style leaderboard.
This record is read from a CSV file in the format:

	matchID,winnerName,loserName,drawFlag,date
	matchID2,winnerName,loserName,drawFlag,date
	...

There is basically no input validation at the moment, so don't screw it up.
Don't include any extra data, nor leading/trailing empty lines.

The matchID and date are not used by the program yet, either.

If the drawFlag-field is greater than zero-characters long,
the match will be understood as a draw.

The best way to generate this data is
probably to use something like google sheets to keep a record
and then export it as a CSV file.
That is what we are doing for the club.


### Compiling

It's a pretty simple program, so nothing convoluted here.
This will be fine:

	gcc *.c


### Running

The program requires a single command-line argument
which is the CSV file to read the match history from.
Just:

	./a.out FILENAME

`test-data/SOMEFILE` is what I have been using for testing.

The output is given as csv data to the `stdout`.


Code Structure, briefly
----------------------------------------

The ladder is just represented as a doubly-linked list,
where you keep track of a pointer to the player who is in first place.

The data-processing mainloop uses a simple fptr-interface to request the data,
so the logic is separate from the table-structure.

* `ladder.c` contains the actual logic of the ladder generation.
* `player.c` is for the player struct and linked-list handling.
* `main.c` has the implementation of the data-fetching interface
  and CLI-interface functionality.

Yeah. Nothing crazy. This is a small thing.

