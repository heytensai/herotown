H=game.h sdl.h point.h sprite.h
SRC=main.cpp sprite.cpp sdl.cpp

sdlapp: ${SRC} ${H}
	g++ -g -o sdlapp ${SRC} `sdl2-config --cflags --libs`

clean:
	rm -f sdlapp
