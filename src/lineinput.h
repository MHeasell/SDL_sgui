#ifndef LINEINPUT_H
#define LINEINPUT_H

#include "frame.h"

namespace Gui
{

class LineInput : public Frame
{
public:
	LineInput(const gwstring &Name, Widget *parent = NULL);
	virtual ~LineInput();

	virtual void setLayout(Layout *layout);
	virtual Layout *getLayout();

	virtual bool canTakeFocus() const;
	virtual int getOptimalWidth() const;
	virtual int getOptimalHeight() const;

protected:
	virtual void draw(SDL_Surface *target);
	virtual void keyPressEvent(SDL_Event *e);
	virtual void mousePressEvent(SDL_Event *e);
	virtual void gainFocus();
	virtual void loseFocus();
	virtual void mouseEnter();
	virtual void mouseLeave();
	virtual void resizeEvent();

private:
	uint32 shift;

	PROPERTY(std::wstring, Text);
	PROPERTY(uint32, CursorPos);
	PROPERTY(bool, Password);
};

inline LineInput &LineInput_(const gwstring &Name, Widget *parent = NULL)
{
	return *(new LineInput(Name, parent));
}

}

#define LINEINPUT(x)	static_cast<Gui::LineInput*>(Gui::Widget::get(#x))

#endif // LINEINPUT_H
