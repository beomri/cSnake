OBJS="game_main.c"

all:
	make linux
	#make win

#regular:
linux:
	gcc  $(OBJS) snake.c -o bin/linux_snake -lSDL2 -lSDL2_ttf -Wall


#for win:
win:
	i686-w64-mingw32-gcc $(OBJS) snake.c -o bin/win_snake.exe -mwindows -lmingw32 -lSDL2main -llibSDL2 -llibSDL2_ttf
