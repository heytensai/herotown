#include "video.h"

Video::Video(int width, int height)
{
	renderer = NULL;
	window = NULL;
	set_dimensions(width, height);
}

Video::~Video()
{
	destroy_window();
	if (font != NULL){
		TTF_CloseFont(font);
		font = NULL;
	}
	TTF_Quit();
	SDL_Quit();
}

Uint32 Video::pitch_offset(int x, int y)
{
	// why does pitch == width * 2?
	return (x + (y * width * 2));
}

void Video::set_pixel(int x, int y, int r, int g, int b)
{
/*
	if (pixels == NULL){
		return;
	}
	int offset = pitch_offset(x, y);
	Uint32 p = map_rgba(r, g, b, 0);
	pixels[offset] = p;
*/
}

void Video::set_dimensions(int width, int height)
{
	this->width = width;
	this->height = height;

	//TODO: resize
}

void Video::destroy_window()
{
	if (renderer){
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}
	if (window){
		SDL_DestroyWindow(window);
		window = NULL;
	}
}

Uint32 Video::map_rgba(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	Uint32 p = 0;
	p = ((r << 0) & 0xff) + ((g & 0xff) << 8) + ((b & 0xff) << 16) + ((a & 0xff) << 24);
	return p;
}

void Video::create_window()
{
	destroy_window();

	window = SDL_CreateWindow(APPNAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_RESIZABLE);
	if (window == NULL){
		fprintf(stderr, "E: video init: %s\n", SDL_GetError());
		exit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
}

SDL_Texture *Video::make_texture_from_image(const char *file)
{
	SDL_Surface *tmp = NULL;
	tmp = IMG_Load(file);

	if (tmp == NULL){
		fprintf(stderr, "failed to load sprite image\n");
		return NULL;
	}

	SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, tmp);
	if (t == NULL){
		fprintf(stderr, "failed to create sprite texture: %s\n", SDL_GetError());
	}
	SDL_FreeSurface(tmp);

	return t;
}

void Video::load_background(const char *file)
{
	if (background){
		SDL_DestroyTexture(background);
		background = NULL;
	}
	background = make_texture_from_image(file);
}

void Video::quit()
{
	destroy_window();
	SDL_Quit();
}

void Video::start_render()
{
	SDL_RenderClear(renderer);
}

void Video::finish_render()
{
	SDL_RenderPresent(renderer);
}

void Video::blit_background()
{
	SDL_RenderCopy(renderer, background, NULL, NULL);
}

void Video::init()
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0){
		fprintf(stderr, "E: video init: %s\n", SDL_GetError());
		exit(1);
	}
	init_font();
}

void Video::render_text(int x, int y, const char *text)
{
	SDL_Color color = {0, 0, 0};
	SDL_Surface *f = TTF_RenderUTF8_Blended(font, text, color);
	SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, f);

	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = f->w;
	src.h = f->h;
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = f->w;
	dst.h = f->h;
	SDL_RenderCopy(renderer, t, &src, &dst);

	SDL_FreeSurface(f);
	SDL_DestroyTexture(t);
}

void Video::init_font()
{
	TTF_Init();
	font = TTF_OpenFont("resources/DejaVuSans-Bold.ttf", 30);
}

