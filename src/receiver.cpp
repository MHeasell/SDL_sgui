#include "receiver.h"
#include "widget.h"

using namespace std;

namespace Gui
{

	Receiver::Receiver()
	{
	}

	Receiver::~Receiver()
	{
		for(set<Widget*>::iterator i = emitters.begin() ; i != emitters.end() ; ++i)
			(*i)->removeListener(this);
	}

	void Receiver::proc(const std::wstring &)
	{
	}

	void Receiver::proc(Widget *widget)
	{
		proc(widget->getName());
	}

}
