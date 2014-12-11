H=game.h video.h point.h sprite.h sound.h input.h hero.h bomb.h menu.h
SRC=main.cpp sprite.cpp video.cpp sound.cpp game.cpp hero.cpp bomb.cpp menu.cpp
APP=herotown

${APP}: ${SRC} ${H}
	g++ -Wall -g -o ${APP} ${SRC} `sdl2-config --cflags --libs` -lSDL2_mixer -l SDL2_image -lSDL2_ttf

clean:
	rm -f ${APP}
