H=game.h video.h point.h sprite.h sound.h
SRC=main.cpp sprite.cpp video.cpp sound.cpp game.cpp

sdlapp: ${SRC} ${H}
	g++ -g -o sdlapp ${SRC} `sdl2-config --cflags --libs` -lSDL2_mixer -l SDL2_image

clean:
	rm -f sdlapp
