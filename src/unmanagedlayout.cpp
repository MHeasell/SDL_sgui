
#include <SDL/sgui/sdl-headers.h>
#include <SDL/sgui/unmanagedlayout.h>
#include <SDL/sgui/widget.h>
#include <vector>
#include <utility>

using namespace std;

namespace Gui
{

	UnmanagedLayout::UnmanagedLayout()
	{
	}

	UnmanagedLayout::~UnmanagedLayout()
	{
	}

	int UnmanagedLayout::getOptimalWidth() const
	{
		if (parent->childs.empty())
			return parent->w;
		int w = 0;
		for(set<Widget*>::iterator i = parent->childs.begin() ; i != parent->childs.end() ; ++i)
			w = max(w, (*i)->getX() + (*i)->getOptimalWidth());
		return w == 0 ? -1 : w;
	}

	int UnmanagedLayout::getOptimalHeight() const
	{
		if (parent->childs.empty())
			return parent->h;
		int h = 0;
		for(set<Widget*>::iterator i = parent->childs.begin() ; i != parent->childs.end() ; ++i)
			h += max(h, (*i)->getY() + (*i)->getOptimalHeight());
		return h == 0 ? -1 : h;
	}

	void UnmanagedLayout::operator()()
	{
		for(map<Widget*, uint32>::iterator i = wmap.begin() ; i != wmap.end() ; ++i)
			i->first->resize(i->first->getOptimalWidth(), i->first->getOptimalHeight());
	}
}
