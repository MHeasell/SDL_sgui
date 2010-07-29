#include "unmanagedlayout.h"
#include "widget.h"
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

	void UnmanagedLayout::operator()()
	{
		for(map<Widget*, uint32>::iterator i = wmap.begin() ; i != wmap.end() ; ++i)
			i->first->resize(i->first->getOptimalWidth(), i->first->getOptimalHeight());
	}
}
