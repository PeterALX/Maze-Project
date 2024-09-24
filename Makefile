flags = -Wall -Werror -Wextra -pedantic
game: *.c *.h
	gcc *.c -lSDL2 -lm -o game $(flags) 
