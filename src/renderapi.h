#ifndef RENDERAPI_H
#define RENDERAPI_H

#include "types.h"

struct SDL_Surface;
struct SDL_Cursor;

namespace Gui
{

	SDL_Surface *createNativeSurface(int w, int h);
	SDL_Surface *createSubSurface(SDL_Surface *src, int x, int y, int w, int h);
	void blit(SDL_Surface *src, SDL_Surface *dst, int x, int y);
	void blit(SDL_Surface *src, SDL_Surface *dst, int x0, int y0, int x1, int y1, int w, int h);
	void line(SDL_Surface *dst, int x0, int y0, int x1, int y1, uint32 col);
	void vline(SDL_Surface *dst, int x, int y0, int y1, uint32 col);
	void hline(SDL_Surface *dst, int y, int x0, int x1, uint32 col);
	void box(SDL_Surface *dst, int x0, int y0, int x1, int y1, uint32 col);
	void fillbox(SDL_Surface *dst, int x0, int y0, int x1, int y1, uint32 col);
	void roundedbox(SDL_Surface *dst, int x0, int y0, int x1, int y1, int r, uint32 col);
	void fillroundedbox(SDL_Surface *dst, int x0, int y0, int x1, int y1, int r, uint32 col);
	void circle(SDL_Surface *dst, int x, int y, int r, uint32 col);
	void fillcircle(SDL_Surface *dst, int x, int y, int r, uint32 col);
	void arc(SDL_Surface *dst, int x, int y, int r, int start, int end, uint32 col);
	void putpixel(SDL_Surface *dst, int x, int y, uint32 col);
	uint32 getpixel(SDL_Surface *dst, int x, int y);
	void gradientbox(SDL_Surface *dst, int x0, int y0, int x1, int y1, float dx, float dy, uint32 col, uint32 dcol);
	void roundedgradientbox(SDL_Surface *dst, int x0, int y0, int x1, int y1, int r, float dx, float dy, uint32 col, uint32 dcol);
	void gradientcircle(SDL_Surface *dst, int x, int y, int r, float dx, float dy, uint32 col, uint32 dcol);
	void fill(SDL_Surface *dst, uint32 col);
	bool compareSurfaces(SDL_Surface *src, SDL_Surface *dst, int x, int y);
	void vwhitealphagradientbox(SDL_Surface *dst, int x0, int y0, int x1, int y1);

	void updateGUIColors();
	void initCursors();
	SDL_Cursor *loadCursor(const char *image, int hot_x, int hot_y);

	extern uint32 black;
	extern uint32 darkgrey;
	extern uint32 grey;
	extern uint32 lightgrey;
	extern uint32 verylightgrey;
	extern uint32 white;
	extern uint32 blue;
	extern uint32 red;
	extern uint32 green;
	extern uint32 yellow;
	extern uint32 darkblue;
	extern uint32 darkgreen;
	extern uint32 darkred;
	extern uint32 darkyellow;
	extern uint32 lightblue;
	extern uint32 lightgreen;
	extern uint32 lightred;
	extern uint32 lightyellow;
	extern SDL_Cursor *cursor_arrow;
	extern SDL_Cursor *cursor_edit;
}

#endif // RENDERAPI_H
