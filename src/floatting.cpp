#include "floatting.h"
#include "window.h"

namespace Gui
{

	Floatting::Floatting(const ustring &Name, Widget *parent) : Widget(Name, parent), wnd(NULL)
	{
	}

	void Floatting::refresh(bool chain)
	{
		Widget::refresh(chain);
		if (wnd)
			wnd->refresh(true);
	}

	Widget *Floatting::getRoot()
	{
		if (wnd)
			return wnd;
		return Widget::getRoot();
	}

	void Floatting::hide()
	{
		if (wnd)
		{
			wnd->removeFloatting(this);
			onHide();
		}
	}
}
