H=game.h video.h point.h sprite.h sound.h input.h
SRC=main.cpp sprite.cpp video.cpp sound.cpp game.cpp
APP=herotown

${APP}: ${SRC} ${H}
	g++ -g -o ${APP} ${SRC} `sdl2-config --cflags --libs` -lSDL2_mixer -l SDL2_image

clean:
	rm -f ${APP}
