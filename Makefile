H=include/bombycoinygame.h include/video.h include/point.h include/sprite.h include/sound.h include/input.h include/hero.h include/bomb.h include/menu.h include/game.h include/jumpygame.h
SRC=src/main.cpp src/sprite.cpp src/video.cpp src/sound.cpp src/bombycoinygame.cpp src/hero.cpp src/bomb.cpp src/menu.cpp src/game.cpp src/jumpygame.cpp
APP=herotown

${APP}: ${SRC} ${H}
	g++ -Iinclude -Wall -g -o ${APP} ${SRC} `sdl2-config --cflags --libs` -lSDL2_mixer -l SDL2_image -lSDL2_ttf

clean:
	rm -f ${APP}
