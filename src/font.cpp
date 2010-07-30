#include "font.h"
#include "fontdata.h"
#include "renderapi.h"

#define _X0(surface)	((surface)->clip_rect.x)
#define _Y0(surface)	((surface)->clip_rect.y)
#define _X1(surface)	((surface)->clip_rect.x + (surface)->clip_rect.w)
#define _Y1(surface)	((surface)->clip_rect.y + (surface)->clip_rect.h)

using namespace std;

namespace Gui
{

	Font *Font::pInstance = NULL;

	Font *Font::Instance()
	{
		if (pInstance)
			return pInstance;
		return pInstance = new Font;
	}

	Font::Font()
	{
		SDL_Surface *bmp = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 8, 0, 0, 0xFF, 0);
		const char *data = header_data;
		for(unsigned int y = 0 ; y < height ; ++y)
		{
			for(unsigned int x = 0 ; x < width ; ++x)
			{
				int c[3];
				HEADER_PIXEL(data, c);
				((unsigned char*)bmp->pixels)[y * bmp->pitch + x] = 0xFF - (c[0] + c[1] + c[2]) / 3;
			}
		}

		const wchar_t chars[] = { L'a',L'b',L'c',L'd',L'e',L'f',L'g',L'h',L'i',L'j', L'k',L'l',L'm',L'n',L'o',L'p',L'q',L'r',L's',L't',L'u',L'v', L'w',L'x',L'y',L'z',
								  L'A',L'B',L'C',L'D',L'E',L'F',L'G',L'H',L'I',L'J', L'K',L'L',L'M',L'N',L'O',L'P',L'Q',L'R',L'S',L'T',L'U',L'V', L'W',L'X',L'Y',L'Z',
								  L'0',L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9', L',',L';',L'.',L':',L'/',L'!',L'?',L'%',L'&',L'"',L'#',L'\'',L'`',L'¨',L'^',L'@',
								  L'é',L'è',L'ç',L'à',L'€',L'$',L'-',L'+',L'*',L'\\',L'_',L'²',L'°',L'(',L')',L'[',L']',L'{',L'}',L'ù',L'ô',L'û', L'â',L'ê',L'î',L'<',
								  L'>',L'~',L'|',L'ï',L'ö',L'ü',L'ë',L'ÿ',L'ŷ',L'=', L' ',0};
		const int nglyphs = 115;

		for(int i = 0 ; i < nglyphs ; ++i)
		{
			const int y = i / 26;
			const int x = i % 26;
			SDL_Surface *glyph = SDL_CreateRGBSurface(SDL_SWSURFACE, 8, 16, 8, 0, 0, 0xFF, 0);
			blit(bmp, glyph, x * 8, y * 16, 0, 0, 8, 16);
			glyphs[chars[i]] = glyph;
		}

		SDL_FreeSurface(bmp);
	}

	void Font::print(SDL_Surface *bmp, int x, int y, const string &str, unsigned int l)
	{
		Font *font = Instance();
		if (y >= _Y1(bmp) || y + 16 <= _Y0(bmp))
			return;
		for(string::const_iterator i = str.begin() ; i != str.end() ; ++i, x += 8)
		{
			if (font->glyphs.count(*i) == 0)
				continue;
			if (x >= _X1(bmp))
				return;
			drawGlyph(font->glyphs[*i], bmp, x, y, l);
		}
	}

	void Font::print(SDL_Surface *bmp, int x, int y, const wstring &str, unsigned int l)
	{
		Font *font = Instance();
		if (y >= _Y1(bmp) || y + 16 <= _Y0(bmp))
			return;
		for(wstring::const_iterator i = str.begin() ; i != str.end() ; ++i, x += 8)
		{
			if (font->glyphs.count(*i) == 0)
				continue;
			if (x >= _X1(bmp))
				return;
			drawGlyph(font->glyphs[*i], bmp, x, y, l);
		}
	}

	void Font::drawGlyph(SDL_Surface *glyph, SDL_Surface *dst, int x, int y, unsigned int l)
	{
		Uint8 lr, lg, lb, la;
		SDL_GetRGBA(l, dst->format, &lr, &lg, &lb, &la);
		byte *pg = (byte*)glyph->pixels;
		switch(dst->format->BytesPerPixel)
		{
		case 1:
			for(int dy = 0 ; dy < glyph->h ; ++dy)
			{
				const int sy = y + dy;
				if (sy < _Y0(dst) || sy >= _Y1(dst))
				{
					pg += glyph->pitch;
					continue;
				}
				byte *p = (byte*)dst->pixels + sy * dst->pitch + x;
				int	prev = -1;
				for(int dx = 0 ; dx < glyph->w ; ++dx, ++pg, ++p)
				{
					const int alpha = *pg;
					const int sx = x + dx;
					if (sx < _X0(dst) || sx >= _X1(dst) || alpha == 0)
					{
						prev = -1;
						continue;
					}
					if (((alpha << 8) | *p) == prev)
					{
						*p = *(p - 1);
						continue;
					}
					prev = (alpha << 8) | *p;
					Uint8 r, g, b;
					SDL_GetRGB(*p, dst->format, &r, &g, &b);

					r = (r * (0xFF - alpha) + alpha * lr) / 0xFF;
					g = (g * (0xFF - alpha) + alpha * lg) / 0xFF;
					b = (b * (0xFF - alpha) + alpha * lb) / 0xFF;

					*p = SDL_MapRGB(dst->format, r, g, b);
				}
				pg += glyph->pitch - glyph->w;
			}
			break;
		case 2:
			lr = (l >> 11) & 31;
			lg = (l >> 5) & 63;
			lb = l & 31;
			for(int dy = 0 ; dy < glyph->h ; ++dy)
			{
				const int sy = y + dy;
				if (sy < _Y0(dst) || sy >= _Y1(dst))
				{
					pg += glyph->pitch;
					continue;
				}
				uint16 *p = (uint16*)((byte*)dst->pixels + sy * dst->pitch + x * 2);
				for(int dx = 0 ; dx < glyph->w ; ++dx, ++pg, ++p)
				{
					const int alpha = *pg;
					const int sx = x + dx;
					if (sx < _X0(dst) || sx >= _X1(dst) || alpha == 0)
						continue;
					uint16 r, g, b;
					r = (*p >> 11) & 31;
					g = (*p >> 5) & 63;
					b = *p & 31;

					r = (r * (0xFF - alpha) + alpha * lr) / 0xFF;
					g = (g * (0xFF - alpha) + alpha * lg) / 0xFF;
					b = (b * (0xFF - alpha) + alpha * lb) / 0xFF;

					*p = (r << 11) | (g << 5) | b;
				}
				pg += glyph->pitch - glyph->w;
			}
			break;
		case 3:
			lr = l >> 16;
			lg = l >> 8;
			lb = l;
			for(int dy = 0 ; dy < glyph->h ; ++dy)
			{
				const int sy = y + dy;
				if (sy < _Y0(dst) || sy >= _Y1(dst))
				{
					pg += glyph->pitch;
					continue;
				}
				byte *p = (byte*)dst->pixels + sy * dst->pitch + x * 3;
				for(int dx = 0 ; dx < glyph->w ; ++dx, ++pg)
				{
					const int alpha = *pg;
					const int sx = x + dx;
					if (sx < _X0(dst) || sx >= _X1(dst) || alpha == 0)
					{
						p += 3;
						continue;
					}
					*p = (*p * (0xFF - alpha) + alpha * lr) / 0xFF; ++p;
					*p = (*p * (0xFF - alpha) + alpha * lg) / 0xFF; ++p;
					*p = (*p * (0xFF - alpha) + alpha * lb) / 0xFF; ++p;
				}
				pg += glyph->pitch - glyph->w;
			}
			break;
		case 4:
			lr = l;
			lg = l >> 8;
			lb = l >> 16;
			la = l >> 24;
			for(int dy = 0 ; dy < glyph->h ; ++dy)
			{
				const int sy = y + dy;
				if (sy < _Y0(dst) || sy >= _Y1(dst))
				{
					pg += glyph->pitch;
					continue;
				}
				byte *p = (byte*)dst->pixels + sy * dst->pitch + x * 4;
				for(int dx = 0 ; dx < glyph->w ; ++dx, ++pg)
				{
					const int alpha = *pg;
					const int sx = x + dx;
					if (sx < _X0(dst) || sx >= _X1(dst) || alpha == 0)
					{
						p += 4;
						continue;
					}
					*p = (*p * (0xFF - alpha) + alpha * lr) / 0xFF; ++p;
					*p = (*p * (0xFF - alpha) + alpha * lg) / 0xFF; ++p;
					*p = (*p * (0xFF - alpha) + alpha * lb) / 0xFF; ++p;
					*p = (*p * (0xFF - alpha) + alpha * la) / 0xFF; ++p;
				}
				pg += glyph->pitch - glyph->w;
			}
			break;
		};
	}

	bool Font::hasGlyph(wchar_t c)
	{
		return Instance()->glyphs.count(c) == 1;
	}

}
