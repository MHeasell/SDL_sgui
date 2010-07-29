#include "widget.h"
#include "font.h"
#include <SDL/SDL.h>
#include <cmath>

#define RENDERALL()\
switch(dst->format->BytesPerPixel)\
{\
case 1:\
 {\
  RENDER(8);\
  break;\
}\
case 2:\
{\
 RENDER(16);\
 break;\
}\
case 3:\
{\
 RENDER(24);\
 break;\
}\
case 4:\
{\
 RENDER(32);\
 break;\
}\
}

using namespace std;

namespace Gui
{

uint32 Widget::black;
uint32 Widget::darkgrey;
uint32 Widget::grey;
uint32 Widget::lightgrey;
uint32 Widget::verylightgrey;
uint32 Widget::white;
uint32 Widget::blue;
uint32 Widget::red;
uint32 Widget::green;
uint32 Widget::yellow;
uint32 Widget::darkblue;
uint32 Widget::darkgreen;
uint32 Widget::darkred;
uint32 Widget::darkyellow;
uint32 Widget::lightblue;
uint32 Widget::lightgreen;
uint32 Widget::lightred;
uint32 Widget::lightyellow;

SDL_Cursor *Widget::cursor_arrow = NULL;
SDL_Cursor *Widget::cursor_edit = NULL;

void Widget::updateGUIColors()
{
	SDL_Surface *screen = SDL_GetVideoSurface();
	SDL_PixelFormat *format = screen->format;
	black = SDL_MapRGBA(format, 0x0, 0x0, 0x0, 0x0);
	darkgrey = SDL_MapRGBA(format, 0x3F, 0x3F, 0x3F, 0x0);
	grey = SDL_MapRGBA(format, 0x7F, 0x7F, 0x7F, 0x0);
	lightgrey = SDL_MapRGBA(format, 0xCF, 0xCF, 0xCF, 0x0);
	verylightgrey = SDL_MapRGBA(format, 0xDF, 0xDF, 0xDF, 0x0);
	white = SDL_MapRGBA(format, 0xFF, 0xFF, 0xFF, 0x0);
	blue = SDL_MapRGBA(format, 0x0, 0x0, 0xFF, 0x0);
	red = SDL_MapRGBA(format, 0xFF, 0x0, 0x0, 0x0);
	green = SDL_MapRGBA(format, 0x0, 0xFF, 0x0, 0x0);
	yellow = SDL_MapRGBA(format, 0xFF, 0xFF, 0x0, 0x0);
	darkblue = SDL_MapRGBA(format, 0x0, 0x0, 0x7F, 0x0);
	darkred = SDL_MapRGBA(format, 0x7F, 0x0, 0x0, 0x0);
	darkgreen = SDL_MapRGBA(format, 0x0, 0x7F, 0x0, 0x0);
	darkyellow = SDL_MapRGBA(format, 0x7F, 0x7F, 0x0, 0x0);
	lightblue = SDL_MapRGBA(format, 0x7F, 0x7F, 0xFF, 0x0);
	lightred = SDL_MapRGBA(format, 0xFF, 0x7F, 0x7F, 0x0);
	lightgreen = SDL_MapRGBA(format, 0x7F, 0xFF, 0x7F, 0x0);
	lightyellow = SDL_MapRGBA(format, 0xFF, 0xFF, 0x7F, 0x0);
}

SDL_Surface *Widget::createNativeSurface(int w, int h)
{
	SDL_PixelFormat *format = SDL_GetVideoSurface()->format;
	SDL_Surface *buf = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, format->BitsPerPixel, format->Rmask, format->Gmask, format->Bmask, format->Amask);
	if (format->palette)
		SDL_SetPalette(buf, SDL_LOGPAL, format->palette->colors, 0, format->palette->ncolors);
	return buf;
}

SDL_Surface *Widget::createSubSurface(SDL_Surface *src, int x, int y, int w, int h)
{
	SDL_PixelFormat *format = src->format;
	w = min(src->w - x, w);
	h = min(src->h - y, h);
	SDL_Surface *buf = SDL_CreateRGBSurfaceFrom((char*)src->pixels + x * format->BytesPerPixel + y * src->pitch,
												w, h,
												format->BitsPerPixel, src->pitch,
												format->Rmask, format->Gmask, format->Bmask, format->Amask);
	if (format->palette)
		SDL_SetPalette(buf, SDL_LOGPAL, format->palette->colors, 0, format->palette->ncolors);
	return buf;
}

void Widget::blit(SDL_Surface *src, SDL_Surface *dst, int x, int y)
{
	SDL_Rect rsrc, rdst;
	rsrc.x = 0;
	rsrc.y = 0;
	rsrc.w = src->w;
	rsrc.h = src->h;

	rdst.x = x;
	rdst.y = y;
	rdst.w = rsrc.w;
	rdst.h = rsrc.h;

	SDL_BlitSurface(src, &rsrc, dst, &rdst);
}

void Widget::blit(SDL_Surface *src, SDL_Surface *dst, int x0, int y0, int x1, int y1, int w, int h)
{
	SDL_Rect rsrc, rdst;
	rsrc.x = x0;
	rsrc.y = y0;
	rsrc.w = w;
	rsrc.h = h;

	rdst.x = x1;
	rdst.y = y1;
	rdst.w = w;
	rdst.h = h;

	SDL_BlitSurface(src, &rsrc, dst, &rdst);
}

bool Widget::compareSurfaces(SDL_Surface *src, SDL_Surface *dst, int x, int y)
{
	if (src->format->BitsPerPixel != dst->format->BitsPerPixel)
		return true;
	if (x < 0 || y < 0 || src->w + x > dst->w || src->h + y > dst->h)
		return true;
	switch(src->format->BytesPerPixel)
	{
	case 1:
		for(int dy = 0 ; dy < src->h ; ++dy)
		{
			byte *p = (byte*)src->pixels + dy * src->pitch;
			byte *q = (byte*)dst->pixels + (dy + y) * dst->pitch + x;
			for(byte *end = p + src->w ; p != end ; ++p, ++q)
			{
				if (*p != *q)
					return true;
			}
		}
		break;
	case 2:
		x *= 2;
		for(int dy = 0 ; dy < src->h ; ++dy)
		{
			uint16 *p = (uint16*)((byte*)src->pixels + dy * src->pitch);
			uint16 *q = (uint16*)((byte*)dst->pixels + (dy + y) * dst->pitch + x);
			for(uint16 *end = p + src->w ; p != end ; ++p, ++q)
			{
				if (*p != *q)
					return true;
			}
		}
		break;
	case 3:
		for(int dy = 0 ; dy < src->h ; ++dy)
		{
			byte *p = (byte*)src->pixels + dy * src->pitch;
			byte *q = (byte*)dst->pixels + (dy + y) * dst->pitch + x * 3;
			for(byte *end = p + src->w * 3 ; p != end ; ++p, ++q)
			{
				if (*p != *q)
					return true;
			}
		}
		break;
	case 4:
		x *= 4;
		for(int dy = 0 ; dy < src->h ; ++dy)
		{
			uint32 *p = (uint32*)((byte*)src->pixels + dy * src->pitch);
			uint32 *q = (uint32*)((byte*)dst->pixels + (dy + y) * dst->pitch + x);
			for(uint32 *end = p + src->w ; p != end ; ++p, ++q)
			{
				if (*p != *q)
					return true;
			}
		}
		break;
	};

	return false;
}

void Widget::line(SDL_Surface *dst, int x0, int y0, int x1, int y1, uint32 col)
{
#undef RENDER
#define RENDER(BPP)\
	if (abs(x0 - x1) > abs(y0 - y1))\
	{\
	 if (x0 > x1)\
	 {\
	  x0 ^= x1;	x1 ^= x0;	x0 ^= x1;\
								  y0 ^= y1;	y1 ^= y0;	y0 ^= y1;\
														  }\
const int dx = x1 - x0;\
			   const int hdx = dx / 2;\
							   for(int x = x0 ; x <= x1 ; ++x)\
							   {\
								if (x < 0 || x >= dst->w)\
								continue;\
								int y = y0 + ((y1 - y0) * (x - x0) + hdx) / dx;\
										if (y < 0 || y >= dst->h)\
										continue;\
										Font::setPixel##BPP(dst, x, y, col);\
									}\
}\
else if (y0 != y1)\
{\
 if (y0 > y1)\
 {\
  x0 ^= x1;	x1 ^= x0;	x0 ^= x1;\
							  y0 ^= y1;	y1 ^= y0;	y0 ^= y1;\
													  }\
const int dy = y1 - y0;\
			   const int hdy = dy / 2;\
							   for(int y = y0 ; y <= y1 ; ++y)\
							   {\
								if (y < 0 || y >= dst->h)\
								continue;\
								int x = x0 + ((x1 - x0) * (y - y0) + hdy) / dy;\
										if (x < 0 || x >= dst->w)\
										continue;\
										Font::setPixel##BPP(dst, x, y, col);\
									}\
}\
else\
{\
 if (y0 < 0 || y0 >= dst->h || x0 < 0 || x0 >= dst->w)\
 return;\
 Font::setPixel##BPP(dst, x0, y0, col);\
}

	RENDERALL();
}

void Widget::vline(SDL_Surface *dst, int x, int y0, int y1, uint32 col)
{
	if (x < 0 || x >= dst->w)
		return;
	y0 = max(0, y0);
	y1 = min(dst->h - 1, y1);
	if (y0 < dst->h && y1 >= 0)
	{
#undef RENDER
#define RENDER(BPP)\
		for(int y = y0 ; y <= y1 ; ++y)\
				Font::setPixel##BPP(dst, x, y, col);
		RENDERALL();
	}
}

void Widget::hline(SDL_Surface *dst, int y, int x0, int x1, uint32 col)
{
	if (y < 0 || y >= dst->h || x1 < x0)
		return;
	x0 = max(0, x0);
	x1 = min(dst->w - 1, x1);
	if (x0 < dst->w && x1 >= 0)
	{
		byte *p = (byte*)dst->pixels + y * dst->pitch + x0 * dst->format->BytesPerPixel;
		switch(dst->format->BytesPerPixel)
		{
		case 1:
			memset(p, col, x1 - x0 + 1);
			return;
		case 2:
			{
				uint16 *_p = (uint16*)p;
				uint16 *end = _p + x1 - x0 + 1;
				for( ; _p != end ; ++_p)
					*_p = col;
			}
			break;
		case 3:
			{
				byte r = col >> 16;
				byte g = col >> 8;
				byte b = col;
				for(byte *end = p + (x1 - x0 + 1) * 3 ; p != end ; ++p)
				{
					*p = r; ++p;
					*p = g; ++p;
					*p = b;
				}
			}
			break;
		case 4:
			for(uint32 *_p = (uint32*)p, *end = (uint32*)(p + (x1 - x0 + 1) * 4) ; _p != end ; ++_p)
				*_p = col;
			break;
		}
	}
}

void Widget::box(SDL_Surface *dst, int x0, int y0, int x1, int y1, uint32 col)
{
	hline(dst, y0, x0, x1, col);
	hline(dst, y1, x0, x1, col);
	vline(dst, x0, y0, y1, col);
	vline(dst, x1, y0, y1, col);
}

void Widget::fillbox(SDL_Surface *dst, int x0, int y0, int x1, int y1, uint32 col)
{
	SDL_Rect rect;
	rect.x = x0;
	rect.y = y0;
	rect.w = x1 - x0 + 1;
	rect.h = y1 - y0 + 1;
	SDL_FillRect(dst, &rect, col);
}

void Widget::roundedbox(SDL_Surface *dst, int x0, int y0, int x1, int y1, int r, uint32 col)
{
	hline(dst, y0, x0 + r, x1 - r, col);
	hline(dst, y1, x0 + r, x1 - r, col);
	vline(dst, x0, y0 + r, y1 - r, col);
	vline(dst, x1, y0 + r, y1 - r, col);
	arc(dst, x0 + r, y0 + r, r, 180, 270, col);
	arc(dst, x1 - r, y0 + r, r, 270, 360, col);
	arc(dst, x0 + r, y1 - r, r, 90, 180, col);
	arc(dst, x1 - r, y1 - r, r, 0, 90, col);
}

void Widget::fillroundedbox(SDL_Surface *dst, int x0, int y0, int x1, int y1, int r, uint32 col)
{
	fillbox(dst, x0, y0 + r, x1, y1 - r, col);
	fillbox(dst, x0 + r, y0, x1 - r, y0 + r, col);
	fillbox(dst, x0 + r, y1 - r, x1 - r, y1, col);
	fillcircle(dst, x0 + r, y0 + r, r, col);
	fillcircle(dst, x1 - r, y0 + r, r, col);
	fillcircle(dst, x0 + r, y1 - r, r, col);
	fillcircle(dst, x1 - r, y1 - r, r, col);
}

void Widget::circle(SDL_Surface *dst, int x, int y, int r, uint32 col)
{
	const int r2 = r * r;
	const int hr = int(r * M_SQRT2 * 0.5f + 0.5f);
	for(int _r = 0 ; _r <= hr ; ++_r)
	{
		const int dx = int(sqrt(float(r2 - _r * _r)) + 0.5f);
		putpixel(dst, x - dx, y - _r, col);
		putpixel(dst, x + dx, y - _r, col);
		putpixel(dst, x - dx, y + _r, col);
		putpixel(dst, x + dx, y + _r, col);
		putpixel(dst, x - _r, y - dx, col);
		putpixel(dst, x + _r, y - dx, col);
		putpixel(dst, x - _r, y + dx, col);
		putpixel(dst, x + _r, y + dx, col);
	}
}

void Widget::fillcircle(SDL_Surface *dst, int x, int y, int r, uint32 col)
{
	const int r2 = r * r;
	const int hr = int(r * M_SQRT2 * 0.5f + 0.5f);
	for(int _r = 0 ; _r <= hr ; ++_r)
	{
		const int dx = int(sqrt(float(r2 - _r * _r)) + 0.5f);
		hline(dst, y - _r, x - dx, x + dx, col);
		hline(dst, y + _r, x - dx, x + dx, col);
		hline(dst, y - dx, x - _r, x + _r, col);
		hline(dst, y + dx, x - _r, x + _r, col);
	}
}

void Widget::gradientcircle(SDL_Surface *dst, int x, int y, int radius, float dx, float dy, uint32 col, uint32 dcol)
{
	Uint8 r, g, b;
	Uint8 dr, dg, db;
	SDL_GetRGB(col, dst->format, &r, &g, &b);
	SDL_GetRGB(dcol, dst->format, &dr, &dg, &db);

	const int r2 = radius * radius;
	const int hr = int(radius * M_SQRT2 * 0.5f + 0.5f);
	for(int _r = 0 ; _r <= hr ; ++_r)
	{
		const int _dx = int(sqrt(float(r2 - _r * _r)) + 0.5f);
#undef DRAW_LINE
#define DRAW_LINE()\
		for(int _x = _x0 ; _x <= _x1 ; ++_x)\
		{\
			const float d = (x - _x) * dx + (_y - y) * dy;\
			const int nr = clamp(int(r + d * dr), 0, 255);\
			const int ng = clamp(int(g + d * dg), 0, 255);\
			const int nb = clamp(int(b + d * db), 0, 255);\
			putpixel(dst, _x, _y, SDL_MapRGB(dst->format, nr, ng, nb));\
		}
		int _y = y - _r, _x0 = x - _dx, _x1 = x + _dx;
		DRAW_LINE();
		_y = y + _r;
		DRAW_LINE();
		_y = y - _dx, _x0 = x - _r, _x1 = x + _r;
		DRAW_LINE();
		_y = y + _dx;
		DRAW_LINE();
	}
}

void Widget::arc(SDL_Surface *dst, int x, int y, int r, int start, int end, uint32 col)
{
	const int r2 = r * r;
	const int hr = int(r * M_SQRT2 * 0.5f + 0.5f);
	const float sx = sin(start * M_PI / 180.0);
	const float sy = -cos(start * M_PI / 180.0);
	const float ex = -sin(end * M_PI / 180.0);
	const float ey = cos(end * M_PI / 180.0);
	for(int _r = 0 ; _r <= hr ; ++_r)
	{
		const int dx = int(sqrt(float(r2 - _r * _r)) + 0.5f);
#define DRAW_PIXEL(X, Y)\
		{\
		 const int _x = X;\
						const int _y = Y;\
									   if (sx * (_x - x) + sy * (_y - y) <= 0.0f\
										   && ex * (_x - x) + ey * (_y - y) <= 0.0f)\
									   putpixel(dst, _x, _y, col);\
								   }

		DRAW_PIXEL(x - dx, y - _r);
		DRAW_PIXEL(x + dx, y - _r);
		DRAW_PIXEL(x - dx, y + _r);
		DRAW_PIXEL(x + dx, y + _r);
		DRAW_PIXEL(x - _r, y - dx);
		DRAW_PIXEL(x + _r, y - dx);
		DRAW_PIXEL(x - _r, y + dx);
		DRAW_PIXEL(x + _r, y + dx);
	}
}

void Widget::putpixel(SDL_Surface *dst, int x, int y, uint32 col)
{
	if (x < 0 || x >= dst->w || y < 0 || y >= dst->h)
		return;
	Font::setPixel(dst, x, y, col);
}

uint32 Widget::getpixel(SDL_Surface *dst, int x, int y)
{
	if (x < 0 || x >= dst->w || y < 0 || y >= dst->h)
		return 0;
	return Font::getPixel(dst, x, y);
}

void Widget::gradientbox(SDL_Surface *dst, int x0, int y0, int x1, int y1, float dx, float dy, uint32 col, uint32 dcol)
{
	const float mx = (x0 + x1) * 0.5f;
	const float my = (y0 + y1) * 0.5f;
	Uint8 r, g, b;
	Uint8 dr, dg, db;
	SDL_GetRGB(col, dst->format, &r, &g, &b);
	SDL_GetRGB(dcol, dst->format, &dr, &dg, &db);
	if (dx == 0.0f)
	{
		for(int y = y0 ; y <= y1 ; ++y)
		{
			const float d = (y - my) * dy;
			const int nr = clamp(int(r + d * dr), 0, 255);
			const int ng = clamp(int(g + d * dg), 0, 255);
			const int nb = clamp(int(b + d * db), 0, 255);
			const uint32 c = SDL_MapRGB(dst->format, nr, ng, nb);
			hline(dst, y, x0, x1, c);
		}
		return;
	}
	if (dy == 0.0f)
	{
		for(int x = x0 ; x <= x1 ; ++x)
		{
			const float d = (x - mx) * dx;
			const int nr = clamp(int(r + d * dr), 0, 255);
			const int ng = clamp(int(g + d * dg), 0, 255);
			const int nb = clamp(int(b + d * db), 0, 255);
			const uint32 c = SDL_MapRGB(dst->format, nr, ng, nb);
			vline(dst, x, y0, y1, c);
		}
		return;
	}
#undef RENDER
#define RENDER(BPP)\
	for(int y = y0 ; y <= y1 ; ++y)\
	{\
		if (y < 0 || y >= dst->h)	continue;\
		for(int x = x0 ; x <= x1 ; ++x)\
		{\
			if (x < 0 || x >= dst->w)	continue;\
			const float d = (x - mx) * dx + (y - my) * dy;\
			const int nr = clamp(int(r + d * dr), 0, 255);\
			const int ng = clamp(int(g + d * dg), 0, 255);\
			const int nb = clamp(int(b + d * db), 0, 255);\
			Font::setPixel##BPP(dst, x, y, SDL_MapRGB(dst->format, nr, ng, nb));\
		}\
	}
	RENDERALL();
}

void Widget::roundedgradientbox(SDL_Surface *dst, int x0, int y0, int x1, int y1, int radius, float dx, float dy, uint32 col, uint32 dcol)
{
	const float mx = (x0 + x1) * 0.5f;
	const float my = (y0 + y1) * 0.5f;
	Uint8 r, g, b, a;
	Uint8 dr, dg, db, da;
	SDL_GetRGBA(col, dst->format, &r, &g, &b, &a);
	SDL_GetRGBA(dcol, dst->format, &dr, &dg, &db, &da);
	const int bx0 = max(x0, 0);
	const int bx1 = min(x1, dst->w - 1);
	if (bx1 >= 0 && bx0 < dst->w)
	{
		switch(dst->format->BytesPerPixel)
		{
		case 1:
			for(int y = y0 + radius ; y <= y1 - radius ; ++y)
			{
				if (y < 0 || y >= dst->h)
					continue;
				byte *p = (byte*)dst->pixels + y * dst->pitch + bx0 * 4;
				if (dx == 0.0f)
				{
					const float d = (y - my) * dy;
					const int nr = clamp(int(r + d * dr), 0, 255);
					const int ng = clamp(int(g + d * dg), 0, 255);
					const int nb = clamp(int(b + d * db), 0, 255);
					hline(dst, y, bx0, bx1, SDL_MapRGB(dst->format, nr, ng, nb));
				}
				else
					for(int x = bx0 ; x <= bx1 ; ++x, ++p)
					{
						const float d = (x - mx) * dx + (y - my) * dy;
						const int nr = clamp(int(r + d * dr), 0, 255);
						const int ng = clamp(int(g + d * dg), 0, 255);
						const int nb = clamp(int(b + d * db), 0, 255);
						*p = SDL_MapRGB(dst->format, nr, ng, nb);
					}
			}
			break;
		case 2:
			for(int y = y0 + radius ; y <= y1 - radius ; ++y)
			{
				if (y < 0 || y >= dst->h)
					continue;
				if (dx == 0.0f)
				{
					const float d = (y - my) * dy;
					const int nr = clamp(int(r + d * dr), 0, 255);
					const int ng = clamp(int(g + d * dg), 0, 255);
					const int nb = clamp(int(b + d * db), 0, 255);
					hline(dst, y, bx0, bx1, SDL_MapRGB(dst->format, nr, ng, nb));
				}
				else
					for(int x = bx0 ; x <= bx1 ; ++x)
					{
						const float d = (x - mx) * dx + (y - my) * dy;
						const int nr = clamp(int(r + d * dr), 0, 255);
						const int ng = clamp(int(g + d * dg), 0, 255);
						const int nb = clamp(int(b + d * db), 0, 255);
						Font::setPixel16(dst, x, y, SDL_MapRGB(dst->format, nr, ng, nb));
					}
			}
			break;
		case 3:
			for(int y = y0 + radius ; y <= y1 - radius ; ++y)
			{
				if (y < 0 || y >= dst->h)
					continue;
				if (dx == 0.0f)
				{
					const float d = (y - my) * dy;
					const int nr = clamp(int(r + d * dr), 0, 255);
					const int ng = clamp(int(g + d * dg), 0, 255);
					const int nb = clamp(int(b + d * db), 0, 255);
					hline(dst, y, bx0, bx1, SDL_MapRGB(dst->format, nr, ng, nb));
				}
				else
					for(int x = bx0 ; x <= bx1 ; ++x)
					{
						const float d = (x - mx) * dx + (y - my) * dy;
						const int nr = clamp(int(r + d * dr), 0, 255);
						const int ng = clamp(int(g + d * dg), 0, 255);
						const int nb = clamp(int(b + d * db), 0, 255);
						Font::setPixel24(dst, x, y, SDL_MapRGB(dst->format, nr, ng, nb));
					}
			}
			break;
		case 4:
			r = col;
			g = col >> 8;
			b = col >> 16;
			a = col >> 24;
			dr = dcol;
			dg = dcol >> 8;
			db = dcol >> 16;
			da = dcol >> 24;
			for(int y = y0 + radius ; y <= y1 - radius ; ++y)
			{
				if (y < 0 || y >= dst->h)
					continue;
				byte *p = (byte*)dst->pixels + y * dst->pitch + bx0 * 4;
				if (dx == 0.0f)
				{
					const float d = (y - my) * dy;
					const uint32 nr = clamp(int(r + d * dr), 0, 255);
					const uint32 ng = clamp(int(g + d * dg), 0, 255);
					const uint32 nb = clamp(int(b + d * db), 0, 255);
					const uint32 na = clamp(int(a + d * da), 0, 255);
					hline(dst, y, bx0, bx1, nr | (ng << 8) | (nb << 16) | (na << 24));
				}
				else
					for(int x = bx0 ; x <= bx1 ; ++x)
					{
						const float d = (x - mx) * dx + (y - my) * dy;
						*p = clamp(int(r + d * dr), 0, 255);	++p;
						*p = clamp(int(g + d * dg), 0, 255);	++p;
						*p = clamp(int(b + d * db), 0, 255);	++p;
						*p = clamp(int(a + d * da), 0, 255);	++p;
					}
			}
			SDL_GetRGBA(col, dst->format, &r, &g, &b, &a);
			SDL_GetRGBA(dcol, dst->format, &dr, &dg, &db, &da);
			break;
		};
	}
	const int r2 = radius * radius;
	const int hr = int(radius * M_SQRT2 * 0.5f + 0.5f);
	for(int _r = 0 ; _r <= hr ; ++_r)
	{
		const int _dr = int(sqrt(float(r2 - _r * _r)) + 0.5f);
		int _x0 = x0 + radius - _dr;
		int _x1 = x1 - radius + _dr;
		int y = y0 + radius - _r;
#undef DRAW_LINE
#define DRAW_LINE()\
		for(int x = _x0 ; x <= _x1 ; ++x)\
		{\
			const float d = (x - mx) * dx + (y - my) * dy;\
			const int nr = clamp(int(r + d * dr), 0, 255);\
			const int ng = clamp(int(g + d * dg), 0, 255);\
			const int nb = clamp(int(b + d * db), 0, 255);\
			putpixel(dst, x, y, SDL_MapRGB(dst->format, nr, ng, nb));\
		}
		DRAW_LINE();
		y = y1 - radius + _r;
		DRAW_LINE();
		_x0 = x0 + radius - _r;
		_x1 = x1 - radius + _r;
		y = y0 + radius - _dr;
		DRAW_LINE();
		y = y1 - radius + _dr;
		DRAW_LINE();
	}
}

void Widget::fill(SDL_Surface *dst, uint32 col)
{
	SDL_FillRect(dst, NULL, col);
}

void Widget::initCursors()
{
	if (cursor_arrow)
		return;

	static const char *arrow =
			"X                               "
			"XX                              "
			"X.X                             "
			"X..X                            "
			"X...X                           "
			"X....X                          "
			"X.....X                         "
			"X......X                        "
			"X.......X                       "
			"X........X                      "
			"X.....XXXXX                     "
			"X..X..X                         "
			"X.X X..X                        "
			"XX  X..X                        "
			"X    X..X                       "
			"     X..X                       "
			"      X..X                      "
			"      X..X                      "
			"       XX                       "
			"                                "
			"                                "
			"                                "
			"                                "
			"                                "
			"                                "
			"                                "
			"                                "
			"                                "
			"                                "
			"                                "
			"                                "
			"                                ";
	cursor_arrow = loadCursor(arrow, 0, 0);

	static const char *edit =
			"   XXXXX                        "
			"   X...X                        "
			"   XX.XX                        "
			"    X.X                         "
			"    X.X                         "
			"    X.X                         "
			"    X.X                         "
			"    X.X                         "
			"    X.X                         "
			"    X.X                         "
			"    X.X                         "
			"    X.X                         "
			"    X.X                         "
			"    X.X                         "
			"   XX.XX                        "
			"   X...X                        "
			"   XXXXX                        "
			"                                "
			"                                "
			"                                "
			"                                "
			"                                "
			"                                "
			"                                "
			"                                "
			"                                "
			"                                "
			"                                "
			"                                "
			"                                "
			"                                "
			"                                ";
	cursor_edit = loadCursor(edit, 5, 7);
}

SDL_Cursor *Widget::loadCursor(const char *image, int hot_x, int hot_y)
{
	Uint8 data[4*32];
	Uint8 mask[4*32];

	int i = -1;
	for (int row = 0 ; row < 32 ; ++row)
	{
		for (int col = 0 ; col < 32 ; ++col, ++image)
		{
			if (col % 8)
			{
				data[i] <<= 1;
				mask[i] <<= 1;
			}
			else
			{
				++i;
				data[i] = mask[i] = 0;
			}
			switch (*image)
			{
			case 'X':
				data[i] |= 0x01;
				mask[i] |= 0x01;
				break;
			case '.':
				mask[i] |= 0x01;
				break;
			case ' ':
				break;
			}
		}
	}
	return SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
}

void Widget::vwhitealphagradientbox(SDL_Surface *dst, int x0, int y0, int x1, int y1)
{
	SDL_PixelFormat *format = dst->format;
	const int h = y1 - y0 + 1;
	switch(format->BytesPerPixel)
	{
	case 1:
		for(int y = y0 ; y <= y1 ; ++y)
		{
			const int bkg = 0xFF + (0xCF - 0xFF) * y / h;
			const int iy = (h - y) * bkg;
			byte *p = (byte*)dst->pixels + y * dst->pitch + x0;
			int prevp = -1;
			for(int x = x0 ; x <= x1 ; ++x, ++p)
			{
				if (*p == prevp)
				{
					*p = *(p - 1);
					continue;
				}
				prevp = *p;
				Uint8 _r, _g, _b;
				_r = format->palette->colors[*p].r;
				_g = format->palette->colors[*p].g;
				_b = format->palette->colors[*p].b;
				_r = (_r * y + iy) / h;
				_g = (_g * y + iy) / h;
				_b = (_b * y + iy) / h;
				*p = SDL_MapRGB(format, _r, _g, _b);
			}
		}
		break;
	case 2:
		for(int y = y0 ; y <= y1 ; ++y)
		{
			const int bkg = 0xFF + (0xCF - 0xFF) * y / h;
			const int iy6 = (h - y) * bkg >> 2;
			const int iy5 = (h - y) * bkg >> 3;
			uint16 *p = (uint16*)((byte*)dst->pixels + y * dst->pitch + x0 * 2);
			int prevp = -1;
			for(int x = x0 ; x <= x1 ; ++x, ++p)
			{
				if (*p == prevp)
				{
					*p = *(p - 1);
					continue;
				}
				prevp = *p;
				Uint8 _r, _g, _b;
				_r = (*p >> 11) & 31;
				_g = (*p >> 5) & 63;
				_b = *p & 31;
				_r = (_r * y + iy5) / h;
				_g = (_g * y + iy6) / h;
				_b = (_b * y + iy5) / h;
				*p = (uint32(_r) << 11)
					 | (uint32(_g) << 5)
					 | uint32(_b);
			}
		}
		break;
	case 3:
		for(int y = y0 ; y <= y1 ; ++y)
		{
			const int bkg = 0xFF + (0xCF - 0xFF) * y / h;
			const int iy = (h - y) * bkg;
			byte *p = (byte*)dst->pixels + y * dst->pitch + x0 * 3;
			uint32 prev = 0xFFFFFF;
			for(int x = x0 ; x <= x1 ; ++x)
			{
				if (prev == (*((uint32*)p) >> 8))
				{
					*p = *(p - 3);	++p;
					*p = *(p - 3);	++p;
					*p = *(p - 3);	++p;
					continue;
				}
				prev = (*((uint32*)p) >> 8);
				for(int i = 0 ; i < 3 ; ++i, ++p)
					*p = (*p * y + iy) / h;
			}
		}
		break;
	case 4:
		for(int y = y0 ; y <= y1 ; ++y)
		{
			const int bkg = 0xFF + (0xCF - 0xFF) * y / h;
			const int iy = (h - y) * bkg;
			byte *p = (byte*)dst->pixels + y * dst->pitch + x0 * 4;
			uint32 prev = 0;
			for(int x = x0 ; x <= x1 ; ++x)
			{
				if (x > x0 && prev == *(uint32*)p)
				{
					*(uint32*)p = *((uint32*)p - 1);
					p += 4;
					continue;
				}
				prev = *(uint32*)p;
				for(int i = 0 ; i < 4 ; ++i, ++p)
					*p = (*p * y + iy) / h;
			}
		}
		break;
	}
}

}