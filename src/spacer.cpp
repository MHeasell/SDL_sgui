
#include <SDL/sgui/sdl-headers.h>
#include <SDL/sgui/spacer.h>

namespace Gui
{

	Spacer::Spacer(bool vert) : Widget(L""), vert(vert)
	{
	}

	Spacer::~Spacer()
	{
	}

	int Spacer::getOptimalWidth() const
	{
		return vert ? 0 : -1;
	}

	int Spacer::getOptimalHeight() const
	{
		return vert ? -1 : 0;
	}

}
