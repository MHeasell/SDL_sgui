#include "tabwidget.h"
#include "font.h"
#include <SDL/SDL.h>
#include <algorithm>

using namespace std;

namespace Gui
{

	TabWidget::TabWidget(const gwstring &Name, Widget *parent) : Widget(Name, parent)
	{
		CurrentTab = 0;
	}

	TabWidget::~TabWidget()
	{

	}

	int TabWidget::addTab(const std::wstring &title, Widget *widget)
	{
		for(vector<pair<wstring, Widget*> >::iterator i = tabs.begin() ; i != tabs.end() ; ++i)
			if (i->second == widget)
				return i - tabs.begin();
		Widget::addChild(widget);
		tabs.push_back(make_pair(title, widget));
		updateLayout();
		return tabs.size() - 1;
	}

	void TabWidget::removeTab(int idx)
	{
		if (idx < 0 || idx >= int(tabs.size()))
			return;

		Widget::remove(tabs[idx].second);
		tabs.erase(tabs.begin() + idx);
		CurrentTab = clamp<int>(CurrentTab, 0, tabs.size() - 1);
		refresh();
	}

	const wstring &TabWidget::getTabName(int idx)
	{
		if (idx < 0 || idx >= int(tabs.size()))
		{
			static wstring null;
			return null;
		}
		return tabs[idx].first;
	}

	Widget *TabWidget::getTabWidget(int idx)
	{
		if (idx < 0 || idx >= int(tabs.size()))
			return NULL;
		return tabs[idx].second;
	}

	void TabWidget::setTabName(int idx, const std::wstring &Name)
	{
		if (idx < 0 || idx >= int(tabs.size()))
			return;
		tabs[idx].first = Name;
		refresh();
	}

	void TabWidget::setTabWidget(int idx, Widget *widget)
	{
		if (idx < 0 || idx >= int(tabs.size()))
			return;
		tabs[idx].second = widget;
		updateLayout();
	}

	int TabWidget::getOptimalWidth() const
	{
		return -1;
	}

	int TabWidget::getOptimalHeight() const
	{
		return -1;
	}

	void TabWidget::draw(SDL_Surface *target)
	{
		const int r = 4;

		for(int i = 0 ; i < int(tabs.size()) ; ++i)
		{
			if (i == CurrentTab)	continue;
			const int x0 = i * 80 + 2;
			const int x1 = x0 + 80;
			const int y0 = 2;
			const int y1 = 24 + r;
			roundedgradientbox(target, x0, y0, x1 - 1, y1, r, 0.0f, -0.05f, lightgrey - darkgrey, darkgrey);
			arc(target, x0 + r, y0 + 1 + r, r, 180, 270, grey);
			roundedbox(target, x0 + 1, y0 + 1, x1 - 1, y1, r, verylightgrey);
			roundedbox(target, x0, y0, x1 - 1, y1, r, grey);
			vline(target, x1, 1 + y0 + r, y1 - r, darkgrey);
			if (tabs[i].first.empty())
				continue;
			SDL_Surface *sub = createSubSurface(target, x0 + 8, y0 + 4, x1 - x0 - 17, 16);
			Font::print(sub, (sub->w - tabs[i].first.size() * 8) / 2, 0, tabs[i].first, black);
			SDL_FreeSurface(sub);
		}
		if (tabs.empty())
			CurrentTab = 0;

		fillroundedbox(target, 2, 24, w - 3, h - 3, r, lightgrey);
		arc(target, w - 2 - r, h - 3 - r, r, 0, 90, darkgrey);
		arc(target, 2 + r, 25 + r, r, 180, 270, grey);
		roundedbox(target, 3, 25, w - 3, h - 3, r, white);
		roundedbox(target, 2, 24, w - 3, h - 3, r, grey);
		hline(target, h - 2, 3 + r, w - 2 - r, black);
		vline(target, w - 2, 25 + r, h - 2 - r, black);
		arc(target, w - 2 - r, h - 2 - r, r, 0, 90, black);

		const int x0 = 2 + CurrentTab * 80;
		const int x1 = x0 + 80;
		const int y0 = 0;
		const int y1 = 24;
		fillroundedbox(target, x0, y0, x1 - 1, y1 + r, r, lightgrey);
		arc(target, x0 + r + 1, y0 + 1 + r, r, 180, 270, white);
		arc(target, x1 - r - 1, y0 + 1 + r, r, 270, 360, white);
		hline(target, y0 + 1, x0 + r, x1 - r, white);
		vline(target, x0 + 1, y0 + r, CurrentTab == 0 ? y1 + 3 : y1, white);

		arc(target, x0 + r, y0 + r, r, 180, 270, grey);
		arc(target, x1 - r, y0 + r, r, 270, 360, grey);
		hline(target, y0, x0 + r, x1 - r, grey);
		vline(target, x0, y0 + r, CurrentTab == 0 ? y1 + 2 : y1, grey);
		vline(target, x1 - 1, y0 + r, y1 + 1, grey);

		vline(target, x1, 1 + y0 + r, y1, darkgrey);
		if (!tabs.empty() && !tabs[CurrentTab].first.empty())
		{
			SDL_Surface *sub = createSubSurface(target, x0 + 8, y0 + 4, x1 - x0 - 17, 16);
			Font::print(sub, (sub->w - tabs[CurrentTab].first.size() * 8) / 2, 0, tabs[CurrentTab].first, black);
			SDL_FreeSurface(sub);
		}
	}

	void TabWidget::updateLayout()
	{
		if (bUpdatingLayout)
			return;

		if (tabs.empty())
			return;

		bUpdatingLayout = true;

		tabs[CurrentTab].second->w = w - 16;
		tabs[CurrentTab].second->h = h - 40;
		tabs[CurrentTab].second->x = 8;
		tabs[CurrentTab].second->y = 32;
		tabs[CurrentTab].second->updateLayout();

		if (parent)
			parent->updateLayout();

		refresh();

		bUpdatingLayout = false;
	}

	void TabWidget::event(SDL_Event *e)
	{
		if (tabs.empty())
			return;
		SDL_Event ev = *e;

		switch (ev.type)
		{
		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			if (!tabs.empty())
			{
				if (getMouseX(e) < 8 || getMouseX(e) >= w - 8 || getMouseY(e) < 32 || getMouseY(e) >= h - 8)
				{
					if (tabs[CurrentTab].second->bMouseIn)
					{
						tabs[CurrentTab].second->bMouseIn = false;
						tabs[CurrentTab].second->mouseLeave();
					}
				}
				else if (!tabs[CurrentTab].second->bMouseIn)
				{
					tabs[CurrentTab].second->bMouseIn = true;
					tabs[CurrentTab].second->mouseEnter();
				}
			}
			if (getMouseX(e) < 8 || getMouseX(e) >= w - 8 || getMouseY(e) < 32 || getMouseY(e) >= h - 8)
			{
				if (e->type == SDL_MOUSEBUTTONDOWN)
					mousePressEvent(e);
				else if (e->type == SDL_MOUSEBUTTONUP)
					mouseReleaseEvent(e);
				return;
			}
			getMouseX(ev) -= tabs[CurrentTab].second->x;
			getMouseY(ev) -= tabs[CurrentTab].second->y;
			break;
		}
		tabs[CurrentTab].second->event(&ev);
	}

	void TabWidget::paint(SDL_Surface *target)
	{
		if (bRefresh)
		{
			draw(target);
			if (!tabs.empty())
			{
				tabs[CurrentTab].second->bRefresh = true;
				tabs[CurrentTab].second->bRefreshChain = true;
			}
			bRefreshChain = true;
		}
		if (bRefreshChain && !tabs.empty()
			&& (tabs[CurrentTab].second->bRefresh || tabs[CurrentTab].second->bRefreshChain))
		{
			SDL_Surface *sub = createSubSurface(target, tabs[CurrentTab].second->x, tabs[CurrentTab].second->y, tabs[CurrentTab].second->w, tabs[CurrentTab].second->h);
			tabs[CurrentTab].second->paint(sub);
			SDL_FreeSurface(sub);
		}
		bRefresh = false;
		bRefreshChain = false;
	}

	void TabWidget::mousePressEvent(SDL_Event *e)
	{
		if (e->button.button != SDL_BUTTON_LEFT)
			return;
		if (e->button.x >= 2 && e->button.y >= 2 && e->button.y <= 24)
		{
			const int idx = (e->button.x - 2) / 80;
			setCurrentTab(idx);
		}
	}

	void TabWidget::onSetCurrentTab()
	{
		CurrentTab = clamp<int>(CurrentTab, 0, tabs.size() - 1);
		tabs[CurrentTab].second->w = w - 16;
		tabs[CurrentTab].second->h = h - 16;
		tabs[CurrentTab].second->x = 8;
		tabs[CurrentTab].second->y = 32;
		tabs[CurrentTab].second->updateLayout();
		updateLayout();
	}

	void TabWidget::remove(Widget *widget)
	{
		for(int i = 0 ; i < int(tabs.size()) ; ++i)
			if (tabs[i].second == widget)
			{
				removeTab(i);
				return;
			}
	}
}
