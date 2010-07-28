#ifndef HBOXLAYOUT_H
#define HBOXLAYOUT_H

#include "layout.h"

namespace Gui
{

	class HBoxLayout : public Layout
{
public:
    HBoxLayout();
	virtual ~HBoxLayout();

protected:
	virtual void operator ()();
};

}

#endif // HBOXLAYOUT_H
