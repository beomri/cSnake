OBJS="snake.c"

all:
	make linux
	make win

#regular:
linux:
	gcc  $(OBJS) -o bin/linux_snake -lSDL2


#for win:
win:
	i686-w64-mingw32-gcc $(OBJS) -o bin/win_snake.exe -mwindows -lmingw32 -lSDL2main -llibSDL2

