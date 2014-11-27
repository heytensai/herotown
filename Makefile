sdlapp: main.cpp
	g++ -g -o sdlapp main.cpp `sdl-config --cflags --libs`

clean:
	rm -f sdlapp
