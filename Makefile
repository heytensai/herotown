SRC=main.cpp sprite.cpp sdl.cpp

sdlapp: ${SRC}
	g++ -g -o sdlapp ${SRC} `sdl2-config --cflags --libs`

clean:
	rm -f sdlapp
