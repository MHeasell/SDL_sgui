#ifndef WIDGET_H
#define WIDGET_H

#include <set>
#include <map>
#include <vector>
#include "types.h"
#include "unicode.h"

struct SDL_Surface;
struct SDL_Cursor;
union SDL_Event;

namespace Gui
{
	class Layout;
	class Receiver;

	class Widget
	{
		friend class Group;
		friend class TabWidget;
		friend class ScrollArea;
	public:
		enum Event { EVENT_NONE = 0,
					 EVENT_REFRESH };
	public:
		Widget(const gwstring &Name, Widget *parent = NULL);
		virtual ~Widget();

		virtual void resize(int w, int h);

		inline void addChild(Widget &widget) { addChild(&widget); }
		virtual void addChild(Widget *widget);
		virtual void remove(Widget *widget);

		virtual void paint(SDL_Surface *target);
		virtual void refresh(const bool chain = false);

		virtual void updateLayout();

		inline int getX() const {	return x;	}
		inline int getY() const {	return y;	}
		void getAbsolutePos(int &x, int &y) const;
		inline int getWidth() const {	return w;	}
		inline int getHeight() const {	return h;	}

		inline void setWidth(int w)	{	resize(w, h);	}
		inline void setHeight(int h)	{	resize(w, h);	}

		inline void setPos(int x, int y) { this->x = x;	this->y = y;	}

		virtual void setLayout(Layout *layout);
		virtual Layout *getLayout();

		virtual int getOptimalWidth() const;
		virtual int getOptimalHeight() const;

		virtual bool canTakeFocus() const;

		void setName(const gwstring &Name);
		const std::wstring &getName() const;

		inline void addListener(Receiver &receiver)	{	addListener(&receiver);	}
		void addListener(Receiver *receiver);
		inline void removeListener(Receiver &receiver)	{	removeListener(&receiver);	}
		void removeListener(Receiver *receiver);

	#define PROPERTY(type, name)\
	private:\
		type name;\
	public:\
		inline const type &get##name() const { return name; }\
		inline void set##name(const type &v) { this->name = v; refresh(); }

	#define PROPERTY2(type, name)\
	private:\
		type name;\
	public:\
		inline const type &get##name() const { return name; }\
		inline void set##name(const type &v) { this->name = v; onSet##name(); refresh(); }\
		void onSet##name();

	protected:
		virtual void draw(SDL_Surface *target);
		virtual void event(SDL_Event *e);
		virtual void mouseMoveEvent(SDL_Event *e);
		virtual void mousePressEvent(SDL_Event *e);
		virtual void mouseReleaseEvent(SDL_Event *e);
		virtual void keyPressEvent(SDL_Event *e);
		virtual void keyReleaseEvent(SDL_Event *e);
		virtual void mouseEnter();
		virtual void mouseLeave();
		virtual void gainFocus();
		virtual void loseFocus();
		virtual void resizeEvent();
		void takeFocus();
		Widget *getFocus();
		virtual std::vector<Widget*> getGroup();
		virtual void emit();

	public:
		static SDL_Surface *createNativeSurface(int w, int h);
		static SDL_Surface *createSubSurface(SDL_Surface *src, int x, int y, int w, int h);
		static void blit(SDL_Surface *src, SDL_Surface *dst, int x, int y);
		static void blit(SDL_Surface *src, SDL_Surface *dst, int x0, int y0, int x1, int y1, int w, int h);
		static void emitEvent(Event e);
		static void line(SDL_Surface *dst, int x0, int y0, int x1, int y1, uint32 col);
		static void vline(SDL_Surface *dst, int x, int y0, int y1, uint32 col);
		static void hline(SDL_Surface *dst, int y, int x0, int x1, uint32 col);
		static void box(SDL_Surface *dst, int x0, int y0, int x1, int y1, uint32 col);
		static void fillbox(SDL_Surface *dst, int x0, int y0, int x1, int y1, uint32 col);
		static void roundedbox(SDL_Surface *dst, int x0, int y0, int x1, int y1, int r, uint32 col);
		static void fillroundedbox(SDL_Surface *dst, int x0, int y0, int x1, int y1, int r, uint32 col);
		static void circle(SDL_Surface *dst, int x, int y, int r, uint32 col);
		static void fillcircle(SDL_Surface *dst, int x, int y, int r, uint32 col);
		static void arc(SDL_Surface *dst, int x, int y, int r, int start, int end, uint32 col);
		static void putpixel(SDL_Surface *dst, int x, int y, uint32 col);
		static uint32 getpixel(SDL_Surface *dst, int x, int y);
		static void gradientbox(SDL_Surface *dst, int x0, int y0, int x1, int y1, float dx, float dy, uint32 col, uint32 dcol);
		static void roundedgradientbox(SDL_Surface *dst, int x0, int y0, int x1, int y1, int r, float dx, float dy, uint32 col, uint32 dcol);
		static void gradientcircle(SDL_Surface *dst, int x, int y, int r, float dx, float dy, uint32 col, uint32 dcol);
		static void fill(SDL_Surface *dst, uint32 col);
		static void updateGUIColors();
		static bool compareSurfaces(SDL_Surface *src, SDL_Surface *dst, int x, int y);
		static void vwhitealphagradientbox(SDL_Surface *dst, int x0, int y0, int x1, int y1);
		static void initCursors();
		static SDL_Cursor *loadCursor(const char *image, int hot_x, int hot_y);
		static Widget *get(const gwstring &Name);

	public:
		static uint32 black;
		static uint32 darkgrey;
		static uint32 grey;
		static uint32 lightgrey;
		static uint32 verylightgrey;
		static uint32 white;
		static uint32 blue;
		static uint32 red;
		static uint32 green;
		static uint32 yellow;
		static uint32 darkblue;
		static uint32 darkgreen;
		static uint32 darkred;
		static uint32 darkyellow;
		static uint32 lightblue;
		static uint32 lightgreen;
		static uint32 lightred;
		static uint32 lightyellow;
		static SDL_Cursor *cursor_arrow;
		static SDL_Cursor *cursor_edit;

	protected:
		int x, y;
		int w, h;

		Widget *parent;
		Widget *focusWidget;
		std::set<Widget*> childs;

		bool bRefresh;
		bool bRefreshChain;
		bool bFocus;
		bool bMouseIn;

		Layout *layout;

		std::wstring Name;

		bool bUpdatingLayout;

		std::set<Receiver*> listeners;

	private:
		static std::map<std::wstring, Widget*> wtable;
	};

	inline Widget &Widget_(const gwstring &Name, Widget *parent = NULL)
	{
		return *(new Widget(Name, parent));
	}

	uint16 &getMouseX(SDL_Event *e);
	uint16 &getMouseY(SDL_Event *e);
	inline uint16 &getMouseX(SDL_Event &e)	{	return getMouseX(&e);	}
	inline uint16 &getMouseY(SDL_Event &e)	{	return getMouseY(&e);	}
}

#define WIDGET(x)	Gui::Widget::get(#x)

#define LINK(x, y)		x->addListener(y)
#define UNLINK(x, y)	x->removeListener(y)

#endif // WIDGET_H