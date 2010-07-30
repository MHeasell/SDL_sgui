#include "menubar.h"
#include "window.h"
#include "menu.h"
#include "renderapi.h"
#include "layout.h"

using namespace std;

namespace Gui
{

	MenuBar::MenuBar(const ustring &Name, Window *wnd) : Widget(Name, NULL)
	{
		if (wnd)
			wnd->setMenuBar(this);
	}

	MenuBar::~MenuBar()
	{
		if (wnd && wnd->menubar == this)
		{
			wnd->menubar = NULL;
			wnd->refresh();
		}
	}

	int MenuBar::getOptimalWidth() const
	{
		return -1;
	}

	int MenuBar::getOptimalHeight() const
	{
		return 24;
	}

	void MenuBar::setLayout(Layout *layout)
	{
		delete layout;
	}

	void MenuBar::addMenu(Menu *menu)
	{
		Widget::addChild(menu);
	}

	void MenuBar::remove(Menu *menu)
	{
		Widget::remove(menu);
	}

	void MenuBar::draw(SDL_Surface *target)
	{
		gradientbox(target, 0, 0, w - 1, h - 1, 0.0f, -0.5f / h, lightgrey, darkgrey);
	}

	Widget *MenuBar::getRoot()
	{
		if (wnd)
			return wnd;
		return this;
	}

	void MenuBar::refresh(bool chain)
	{
		Widget::refresh(chain);
		if (wnd)
			wnd->refresh(true);
	}
}
