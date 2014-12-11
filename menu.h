#ifndef MENU_H
#define MENU_H

#include "globals.h"
#include "video.h"

class Menu
{
private:
	Video *video;

public:
	bool running;
	int result;
	Menu(Video *video);
	void render();
	void event_loop();

};

#endif /* MENU_H */
