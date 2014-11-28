sdlapp: main.cpp
	g++ -g -o sdlapp main.cpp `sdl2-config --cflags --libs`

clean:
	rm -f sdlapp
