#ifndef FLOATTING_H
#define FLOATTING_H

#include "widget.h"

namespace Gui
{
	class Window;

	class Floatting : public Widget
	{
	public:
		Floatting(const ustring &Name, Widget *parent = NULL);

		void setWindow(Window *wnd)	{	this->wnd = wnd;	}
		Window *getWindow()	{	return wnd;	}

	private:
		Window* wnd;
	};

}

#endif // FLOATTING_H
