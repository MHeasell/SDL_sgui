#include "SDL_sgui.h"

namespace Gui
{
	void Utils::message(const ustring &title, const ustring &msg)
	{
		setenv("SDL_VIDEO_CENTERED", "1", 1);
		Window wnd("", 640, 240, Window::MOVEABLE | Window::RESIZEABLE);
		wnd.setTitle(title);
		Label *label = new Label("", msg);
		Button *button = new Button("", "  ok  ");
		wnd.addChild(Spacer_(true)/ *label / Spacer_(true) / (Spacer_(false) | *button | Spacer_(false)) / Spacer_(true));
		label->setAlignment(Label::CENTER);
		button->addListener(actionCloseWindow());
		wnd.resize(wnd.getOptimalWidth() + 32, wnd.getOptimalHeight() + 16);
		wnd();
		setenv("SDL_VIDEO_CENTERED", "0", 1);
	}

	ustring Utils::input(const ustring &title, const ustring &msg)
	{
		setenv("SDL_VIDEO_CENTERED", "1", 1);
		Window wnd("", 640, 240, Window::MOVEABLE | Window::RESIZEABLE);
		wnd.setTitle(title);
		Label *label = new Label("", msg);
		Button *button = new Button("", "  ok  ");
		LineInput *input = new LineInput("");
		wnd.addChild(Spacer_(true) / *label / Spacer_(true) / *input / Spacer_(true) / (Spacer_(false) | *button | Spacer_(false)) / Spacer_(true));
		label->setAlignment(Label::CENTER);
		button->addListener(actionCloseWindow());
		input->addListener(actionCloseWindow());
		wnd.resize(wnd.getOptimalWidth() + 32, wnd.getOptimalHeight() + 16);
		wnd();
		setenv("SDL_VIDEO_CENTERED", "0", 1);
		return input->getText();
	}

	bool Utils::ask(const ustring &title, const ustring &msg)
	{
		setenv("SDL_VIDEO_CENTERED", "1", 1);
		bool bOk = false;

		Window wnd("", 320, 60, Window::MOVEABLE | Window::RESIZEABLE);
		wnd.setTitle(title);
		Label *label = new Label("", msg);
		Button *ok = new Button("", "    ok    ");
		Button *cancel = new Button("", "  cancel  ");
		wnd.addChild(Spacer_(true) / *label / Spacer_(true) / (Spacer_(false) | *ok | Spacer_(false) | *cancel | Spacer_(false)) / Spacer_(true));
		label->setAlignment(Label::CENTER);
		ok->addListener(actionCloseWindow());
		ok->addListener(actionSetBool(bOk));
		cancel->addListener(actionCloseWindow());
		wnd.resize(wnd.getOptimalWidth() + 32, wnd.getOptimalHeight() + 16);
		wnd();
		setenv("SDL_VIDEO_CENTERED", "0", 1);
		return bOk;
	}

	namespace Actions
	{
		class CloseWindow : public Receiver
		{
		protected:
			virtual void proc(Widget *)
			{
				Widget::emitEvent(Widget::EVENT_CLOSE);
			}
		};

		class SetBool : public Receiver
		{
		public:
			SetBool(bool &b) : b(b)	{}
		protected:
			virtual void proc(Widget *)
			{
				b = true;
			}

		private:
			bool &b;
		};
	}

	Receiver *Utils::actionCloseWindow()
	{
		return new Actions::CloseWindow;
	}

	Receiver *Utils::actionSetBool(bool &b)
	{
		return new Actions::SetBool(b);
	}
}
