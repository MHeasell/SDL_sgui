#ifndef GROUP_H
#define GROUP_H

#include "widget.h"

namespace Gui
{

class Group : public Widget
{
public:
	Group(const gwstring &Name, const gwstring &Caption = gwstring(), Widget *centralWidget = NULL, Widget *parent = NULL);
	virtual ~Group();

	void setCentralWidget(Widget *widget);
	inline void setCentralWidget(Widget &widget)	{	setCentralWidget(&widget);	}
	Widget *getCentralWidget();

	virtual int getOptimalWidth() const;
	virtual int getOptimalHeight() const;

	virtual void updateLayout();

	virtual void setLayout(Layout *layout);
	virtual Layout *getLayout();

protected:
	virtual void draw(SDL_Surface *target);
	virtual std::vector<Widget*> getGroup();

private:
	PROPERTY(std::wstring, Caption)
};

inline Group &Group_(const gwstring &Name, const gwstring &Caption = gwstring(), Widget *centralWidget = NULL, Widget *parent = NULL)
{
	return *(new Group(Name, Caption, centralWidget, parent));
}

inline Group &Group_(const gwstring &Name, const gwstring &Caption, Widget &centralWidget, Widget *parent = NULL)
{
	return *(new Group(Name, Caption, &centralWidget, parent));
}

}

#define GROUP(x)	static_cast<Gui::Group*>(Gui::Widget::get(#x))

#endif // GROUP_H
