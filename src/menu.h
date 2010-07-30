#ifndef MENU_H
#define MENU_H

#include "widget.h"

namespace Gui
{

	class Menu : public Widget
	{
	public:
		Menu(const ustring &Name, const ustring &Caption = ustring(), Widget *parent = NULL);
		virtual ~Menu();

		virtual int getOptimalWidth() const;
		virtual int getOptimalHeight() const;

	protected:
		virtual void draw(SDL_Surface *target);
		virtual void mouseEnter();
		virtual void mouseLeave();
		virtual void mousePressEvent(SDL_Event *e);

	private:
		bool bHighLight;
		PROPERTY(ustring, Caption)
	};

	inline Menu &Menu_(const ustring &Name, const ustring &Caption = ustring(), Widget *parent = NULL)
	{
		return *(new Menu(Name, Caption, parent));
	}
}

#define MENU(x)	static_cast<Gui::Menu*>(Gui::Widget::get(#x))

#endif // MENU_H
