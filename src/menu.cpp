#include "menu.h"
#include "window.h"
#include "font.h"
#include "renderapi.h"

using namespace std;

namespace Gui
{

	Menu::Menu(const ustring &Name, const ustring &Caption, Widget *parent) : Floatting(Name, parent), Caption(Caption)
	{
		resize(80, 64);
	}

	Menu::~Menu()
	{
	}

	int Menu::getOptimalWidth() const
	{
		int w = 0;
		for(set<Widget*>::const_iterator i = childs.begin() ; i != childs.end() ; ++i)
			w = max<int>(w, (*i)->getOptimalWidth());
		return 16 + w;
	}

	int Menu::getOptimalHeight() const
	{
		return 16 + childs.size() * 16;
	}

	void Menu::draw(SDL_Surface *target)
	{
		const int r = 4;
		fillroundedbox(target, 0, 0, w - 1, h - 1, r, lightgrey);
		roundedbox(target, 0, 0, w - 1, h - 1, r, grey);

		arc(target, r + 1, r + 1, r, 180, 270, white);
		arc(target, w - r - 2, r + 1, r, 270, 370, white);
		arc(target, r + 1, h - r - 2, r, 90, 180, white);
		hline(target, 1, r, w - r - 1, white);
		vline(target, 1, r, h - r - 1, white);
	}

	void Menu::mouseEnter()
	{
		bCanBeHidden = true;
		refresh();
	}

	void Menu::mouseLeave()
	{
		if (bCanBeHidden)
		{
			bCanBeHidden = false;
			if (getWindow())
				getWindow()->removeFloatting(this);
		}
		refresh();
	}
}
