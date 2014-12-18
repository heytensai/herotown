H=include/game.h include/video.h include/point.h include/sprite.h include/sound.h include/input.h include/hero.h include/bomb.h include/menu.h
SRC=main.cpp sprite.cpp video.cpp sound.cpp game.cpp hero.cpp bomb.cpp menu.cpp
APP=herotown

${APP}: ${SRC} ${H}
	g++ -Iinclude -Wall -g -o ${APP} ${SRC} `sdl2-config --cflags --libs` -lSDL2_mixer -l SDL2_image -lSDL2_ttf

clean:
	rm -f ${APP}
