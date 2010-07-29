#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "SDL_sgui.h"

#include <iostream>
using namespace std;
using namespace Gui;

class QuitListener : public Receiver
{
protected:
	virtual void proc(Widget *)
	{
		SDL_Event event;
		event.type = SDL_QUIT;
		SDL_PushEvent(&event);
	}
};

class ActionListener : public Receiver
{
protected:
	virtual void proc(const std::wstring &name)
	{
		cout << toUtf8(name) << endl;
	}
};

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTTHREAD | SDL_INIT_TIMER);

	Window wnd("window", 800, 600, Window::RESIZEABLE);
	wnd.setTitle("Hello World!");
	wnd.addChild(ScrollArea_("scroll"));
	wnd.setLayout(new HBoxLayout);
	wnd.setResizeable(true);

	SCROLLAREA(scroll)->setCentralWidget(Button_("ok")
										 / Picture_("", IMG_Load("font.png"))
										 / Label_("", "Hello\nWorld\n! çoin")
										 / Picture_("", IMG_Load("font.png")));

	BUTTON(ok)->setCaption("   oké   ");
	BUTTON(ok)->addListener(new QuitListener);

	wnd();

	return 0;
}
