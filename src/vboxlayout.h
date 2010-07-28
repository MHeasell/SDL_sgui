#ifndef VBOXLAYOUT_H
#define VBOXLAYOUT_H

#include "layout.h"

namespace Gui
{

class VBoxLayout : public Layout
{
public:
	VBoxLayout();
	virtual ~VBoxLayout();

protected:
	virtual void operator ()();
};

}

#endif // VBOXLAYOUT_H
