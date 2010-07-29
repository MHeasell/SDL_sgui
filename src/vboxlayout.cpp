#include "vboxlayout.h"
#include "widget.h"
#include <vector>
#include <algorithm>
#include <typeinfo>

using namespace std;

namespace Gui
{

VBoxLayout::VBoxLayout()
{
}

VBoxLayout::~VBoxLayout()
{
}

void VBoxLayout::operator ()()
{
	vector< pair<Widget*, uint32> > childs;
	for(map<Widget*, uint32>::iterator i = wmap.begin() ; i != wmap.end() ; ++i)
		childs.push_back(*i);

	sort(childs.begin(), childs.end(), comp);

	vector<int> optim;
	int nblanks = 0;
	int th = 0;
	for(vector< pair<Widget*, uint32> >::iterator i = childs.begin() ; i != childs.end() ; ++i)
	{
		optim.push_back(i->first->getOptimalHeight());
		if (optim.back() == -1)
			++nblanks;
		else
			th += optim.back();
	}

	int n = 0;
	float fy0 = 0.0f, fy1;
	for(vector< pair<Widget*, uint32> >::iterator i = childs.begin() ; i != childs.end() ; ++i)
	{
		const int oh = optim[n++];
		if (oh >= 0)
		{
			if (th < parent->getHeight() && nblanks)
				fy1 = fy0 + oh + 1;
			else
				fy1 = fy0 + float(parent->getHeight()) * oh / th;
		}
		else
		{
			if (th < parent->getHeight())
				fy1 = fy0 + float(parent->getHeight() - th) / nblanks;
			else
				fy1 = fy0;
		}
		int y0 = int(fy0 + 0.5f);
		int y1 = int(fy1 + 0.5f);
		fy0 = fy1;

		i->first->setPos(0, y0);
		i->first->resize(parent->getWidth(), y1 - y0);
	}
}

Widget &operator/(Widget *w1, Widget &w2)
{
	if (typeid(*w1) == typeid(Widget) && typeid(*w1->getLayout()) == typeid(VBoxLayout))
	{
		w1->addChild(&w2);
		return *w1;
	}
	Widget *widget = new Widget(L"");
	widget->setLayout(new VBoxLayout);
	widget->addChild(w1);
	widget->addChild(&w2);
	return *widget;
}

}
