H=game.h sdl.h
SRC=main.cpp sprite.cpp sdl.cpp

sdlapp: ${SRC} ${H}
	g++ -g -o sdlapp ${SRC} `sdl2-config --cflags --libs`

clean:
	rm -f sdlapp
