#ifndef MENU_H
#define MENU_H

#include "globals.h"
#include "video.h"

class Menu
{
protected:
	Video *video;

public:
	bool running;
	int result;
	Menu(Video *video);
	void render();
	void event_loop();

};

class IntroMenu : public Menu
{
public:
	int timer_option;
	IntroMenu(Video *video);
	void render();
	void event_loop();
	bool exit();

};

#endif /* MENU_H */
