#ifndef RADIOBUTTON_H
#define RADIOBUTTON_H

#include "widget.h"
#include <string>

namespace Gui
{

class RadioButton : public Widget
{
public:
	RadioButton(const gwstring &Name, const gwstring &Caption, Widget *parent = NULL);
	virtual ~RadioButton();

	virtual int getOptimalWidth() const;
	virtual int getOptimalHeight() const;

protected:
	virtual void draw(SDL_Surface *target);
	virtual void mousePressEvent(SDL_Event *e);

private:
	SDL_Surface *buf;

	PROPERTY2(std::wstring, Caption)
	PROPERTY(bool, State)
};

inline RadioButton &RadioButton_(const gwstring &Name, const gwstring &Caption = gwstring(), Widget *parent = NULL)
{
	return *(new RadioButton(Name, Caption, parent));
}

}

#define RADIOBUTTON(x)	static_cast<Gui::RadioButton*>(Gui::Widget::get(#x))

#endif // RADIOBUTTON_H
