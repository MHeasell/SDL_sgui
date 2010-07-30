#include "menu.h"
#include "font.h"
#include "renderapi.h"

using namespace std;

namespace Gui
{

	Menu::Menu(const ustring &Name, const ustring &Caption, Widget *parent) : Widget(Name, parent), Caption(Caption)
	{
		bHighLight = false;
	}

	Menu::~Menu()
	{
	}

	int Menu::getOptimalWidth() const
	{
		return 16 + Caption.size() * 8;
	}

	int Menu::getOptimalHeight() const
	{
		return 24;
	}

	void Menu::draw(SDL_Surface *target)
	{
		gradientbox(target, 0, 0, w - 1, h - 1, 0.0f, -0.5f / h, lightgrey, darkgrey);
		if (bHighLight)
		{
			roundedgradientbox(target, 4, 2, w - 5, h - 3, 4, 0.0f, 1.0f / h, grey, darkgrey);
			Font::print(target, 8, 4, Caption, white);
		}
		else
			Font::print(target, 8, 4, Caption, black);
	}

	void Menu::mouseEnter()
	{
		bHighLight = true;
		refresh();
	}

	void Menu::mouseLeave()
	{
		bHighLight = false;
		refresh();
	}

	void Menu::mousePressEvent(SDL_Event *e)
	{
	}
}
