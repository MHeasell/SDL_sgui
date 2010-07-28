#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "window.h"
#include "label.h"
#include "button.h"
#include "lineinput.h"
#include "frame.h"
#include "hboxlayout.h"
#include "vboxlayout.h"
#include "spacer.h"
#include "scrollbar.h"
#include "textedit.h"
#include "listbox.h"
#include "picture.h"
#include "checkbox.h"
#include "radiobutton.h"
#include "group.h"
#include "progressbar.h"
#include "spinbox.h"
#include "receiver.h"

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
	virtual void proc(const wstring &name)
	{
		cout << toUtf8(name) << endl;
	}
};

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTTHREAD | SDL_INIT_TIMER);

	Window wnd(L"window", 800, 600);
	wnd.setTitle("Hello World!");
	ListBox_(L"list");
	wnd.addChild(Label_(L"label")
				 / Spacer_(true)
				 / (Group_(L"G1") | Group_(L"G2"))
				 / Spacer_(true)
				 / SpinBox_(L"spin")
				 / (Spacer_(false) | Button_(L"button") | Spacer_(false)));
	wnd.setLayout(new HBoxLayout);
	wnd.setResizeable(true);

	LABEL(label)->setCaption(L"o<");
	LABEL(label)->setAlignment(Label::CENTER);
	LABEL(label)->setVAlignment(Label::MIDDLE);

	GROUP(G1)->setCaption(L"TEST");
	GROUP(G1)->setCentralWidget(RadioButton_(L"R1") | RadioButton_(L"R2") | RadioButton_(L"R3") | RadioButton_(L"R4"));
	GROUP(G2)->setCaption(L"TEST2");
	GROUP(G2)->setCentralWidget((RadioButton_(L"R5") | RadioButton_(L"R6")) / Picture_(L"picture", IMG_Load("font.png")));

	BUTTON(button)->setCaption(L"   pan!   ");
	BUTTON(button)->addListener(new QuitListener);

	ActionListener *listener = new ActionListener;
	RADIOBUTTON(R1)->addListener(listener);
	RADIOBUTTON(R2)->addListener(listener);
	RADIOBUTTON(R3)->addListener(listener);
	RADIOBUTTON(R4)->addListener(listener);
	RADIOBUTTON(R5)->addListener(listener);
	RADIOBUTTON(R6)->addListener(listener);
	BUTTON(button)->addListener(listener);

	RADIOBUTTON(R1)->setCaption(L"test 1");
	RADIOBUTTON(R2)->setCaption(L"test 2");
	RADIOBUTTON(R3)->setCaption(L"test 3");
	RADIOBUTTON(R4)->setCaption(L"test 4");
	RADIOBUTTON(R5)->setCaption(L"test 5");
	RADIOBUTTON(R6)->setCaption(L"test 6");

	LISTBOX(list)->addElement(L"Hello World!");
	LISTBOX(list)->addElement(L"0");
	LISTBOX(list)->addElement(L"1");
	LISTBOX(list)->addElement(L"2");
	LISTBOX(list)->addElement(L"3");
	LISTBOX(list)->addElement(L"4");
	LISTBOX(list)->addElement(L"5");
	LISTBOX(list)->addElement(L"6");
	LISTBOX(list)->addElement(L"7");
	LISTBOX(list)->setSelection(2);

	SPINBOX(spin)->setValue(50.01);
	SPINBOX(spin)->setPrecision(2);

	wnd();

	return 0;
}
