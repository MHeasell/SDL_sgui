#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include "widget.h"

namespace Gui
{

class Scrollbar : public Widget
{
	friend class ListBox;
public:
	enum { Vertical = false, Horizontal = true };
public:
	Scrollbar(const std::wstring &Name, bool orientation, Widget *parent = NULL);
	virtual ~Scrollbar();

	virtual int getOptimalHeight() const;
	virtual int getOptimalWidth() const;

protected:
	virtual void draw(SDL_Surface *target);
	virtual void mousePressEvent(SDL_Event *e);
	virtual void mouseMoveEvent(SDL_Event *e);
	virtual void mouseLeave();

private:
	int highlight;

	PROPERTY(bool, Orientation)
	PROPERTY(int, Minimum)
	PROPERTY(int, Maximum)
	PROPERTY(int, Value)

private:
	static SDL_Surface *arrowleft;
	static SDL_Surface *arrowright;
	static SDL_Surface *arrowup;
	static SDL_Surface *arrowdown;
};

inline Scrollbar &Scrollbar_(const std::wstring &Name, bool orientation, Widget *parent = NULL)
{
	return *(new Scrollbar(Name, orientation, parent));
}

}

#define SCROLLBAR(x)	static_cast<Gui::Scrollbar*>(Gui::Widget::get(L""#x))

#endif // SCROLLBAR_H
