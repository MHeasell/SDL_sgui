#ifndef WINDOW_H
#define WINDOW_H

#include "widget.h"
#include <string>

namespace Gui
{

class Window : public Widget
{
public:
	enum { RESIZEABLE = 1, MOVEABLE = 2, NOFRAME = 4, FULLSCREEN = 8 };
public:
	Window(const gwstring &Name, int w = 640, int h = 480, uint32 flags = RESIZEABLE);
	virtual ~Window();

	virtual void resize(int w, int h);
	virtual void refresh(const bool chain = false);

	inline const std::string &getTitle() const {	return title;	}
	void setTitle(const gwstring &title);

	void setResizeable(bool resizeable);
	inline bool isResizeable() const {	return flags & RESIZEABLE;	}

	void setMoveable(bool moveable);
	inline bool isMoveable() const {	return flags & MOVEABLE;	}

	void setNoFrame(bool noframe);
	inline bool hasNoFrame() const {	return flags & NOFRAME;	}

	void setFullscreen(bool fullscreen);
	inline bool isFullscreen() const {	return flags & FULLSCREEN;	}

private:
	void setSDLVideo();

public:
	void operator()();

protected:
	virtual void draw(SDL_Surface *target);

private:
	std::string title;
	uint32 flags;
	bool refreshInProgress;
	bool layoutUpdateInProgress;
};

inline Window &Window_(const gwstring &Name, int w = 640, int h = 480)
{
	return *(new Window(Name, w, h));
}

}

#define WINDOW(x)	static_cast<Gui::Window*>(Gui::Widget::get(#x))

#endif // WINDOW_H
