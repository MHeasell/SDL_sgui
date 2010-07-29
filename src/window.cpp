#include "window.h"
#include "renderapi.h"
#include <SDL/SDL.h>

namespace Gui
{

Window::Window(const gwstring &Name, int w, int h, uint32 flags) : Widget(Name), flags(flags)
{
	takeFocus();
	this->w = -1;
	this->h = -1;
	refreshInProgress = false;
	layoutUpdateInProgress = false;
	resize(w, h);
}

Window::~Window()
{
	SDL_SetVideoMode(0, 0, 0, 0);
}

void Window::resize(int w, int h)
{
	if (w != this->w || h != this->h)
	{
		this->w = w;
		this->h = h;
		setSDLVideo();
		updateLayout();
	}
}

void Window::draw(SDL_Surface *target)
{
#ifdef __WIN32__
	fill(target, lightgrey);
#else
	gradientbox(target, 0, 0, w - 1, h - 1, 0.0f, -0.46f / h, SDL_MapRGBA(target->format, 0xDC, 0xDC, 0xDC, 0xDC), darkgrey);
#endif
}

void Window::setTitle(const gwstring &title)
{
	this->title = title;
	SDL_WM_SetCaption(this->title.c_str(), NULL);
}

void Window::operator()()
{
	SDL_Event e;
	while(SDL_WaitEvent(&e))
	{
		switch(e.type)
		{
		case SDL_KEYDOWN:			/**< Keys pressed */
		case SDL_KEYUP:				/**< Keys released */
		case SDL_MOUSEBUTTONDOWN:	/**< Mouse button pressed */
		case SDL_MOUSEBUTTONUP:		/**< Mouse button released */
		case SDL_MOUSEMOTION:		/**< Mouse moved */
			event(&e);
			break;

		case SDL_VIDEORESIZE:		/**< User resized video mode */
			if (e.resize.w != w || e.resize.h != h)
			{
				SDL_Surface *screen = SDL_GetVideoSurface();
				if (screen->w < e.resize.w || screen->h < e.resize.h)
				{
					w = e.resize.w;
					h = e.resize.h;
					SDL_Surface *tmp = createNativeSurface(screen->w, screen->h);
					blit(screen, tmp, 0, 0);
					setSDLVideo();
					screen = SDL_GetVideoSurface();
					blit(tmp, screen, 0, 0);
					SDL_Flip(screen);
					SDL_FreeSurface(tmp);
				}
				else
				{
					w = e.resize.w;
					h = e.resize.h;
					updateLayout();
				}
			}
			break;

		case SDL_VIDEOEXPOSE:		/**< Screen needs to be redrawn */
			SDL_Flip(SDL_GetVideoSurface());
			break;
		case SDL_ACTIVEEVENT:		/**< Application loses/gains visibility */
			if (e.active.state == SDL_APPMOUSEFOCUS)
			{
				if (!e.active.gain)
					takeFocus();
			}
			if (e.active.state == SDL_APPACTIVE && e.active.gain)
				SDL_Flip(SDL_GetVideoSurface());
			break;

		case SDL_QUIT:				/**< User-requested quit */
			return;

		case SDL_SYSWMEVENT:		/**< System specific event */
			break;

		case SDL_USEREVENT:
			switch(e.user.code)
			{
			case EVENT_REFRESH:
				paint(SDL_GetVideoSurface());
				SDL_Flip(SDL_GetVideoSurface());
				refreshInProgress = false;
				break;
			};
			break;

		case SDL_JOYAXISMOTION:		/**< Joystick axis motion */
		case SDL_JOYBALLMOTION:		/**< Joystick trackball motion */
		case SDL_JOYHATMOTION:		/**< Joystick hat position change */
		case SDL_JOYBUTTONDOWN:		/**< Joystick button pressed */
		case SDL_JOYBUTTONUP:		/**< Joystick button released */
		default:
			break;
		}
	}
}

void Window::refresh(const bool chain)
{
	if (chain)
		bRefreshChain = true;
	else
		bRefresh = true;
	if (!refreshInProgress)
		emitEvent(EVENT_REFRESH);
	refreshInProgress = true;
}

void Window::setResizeable(bool resizeable)
{
	if (resizeable == isResizeable())
		return;
	flags ^= RESIZEABLE;
	setSDLVideo();
}

void Window::setMoveable(bool moveable)
{
	if (moveable == isMoveable())
		return;
	flags ^= MOVEABLE;
}

void Window::setNoFrame(bool noframe)
{
	if (noframe == hasNoFrame())
		return;
	flags ^= NOFRAME;
	setSDLVideo();
}

void Window::setFullscreen(bool fullscreen)
{
	if (fullscreen == isFullscreen())
		return;
	flags ^= FULLSCREEN;
	setSDLVideo();
}

void Window::setSDLVideo()
{
	Uint32 flags = SDL_SWSURFACE | SDL_ANYFORMAT;
	if (this->flags & RESIZEABLE)
		flags |= SDL_RESIZABLE;
	if (this->flags & NOFRAME)
		flags |= SDL_NOFRAME;
	if (this->flags & FULLSCREEN)
		flags |= SDL_FULLSCREEN;
	SDL_SetVideoMode(w, h, 32, flags);
	SDL_EnableUNICODE(SDL_ENABLE);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	SDL_SetCursor(cursor_arrow);
	updateGUIColors();
	updateLayout();
}

}
