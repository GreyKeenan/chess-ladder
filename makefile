
inputFile= test-data.csv

outputFile= generate-ladder.out


default:
	gcc -o $(outputFile) main.c ladder.c player.c

run:
	./$(outputFile) $(inputFile)
