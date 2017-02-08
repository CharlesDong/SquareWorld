#include "widget_manager.h"
#include "main/game.h"
#include <cstring>

WidgetManager::WidgetManager()
{
}

void WidgetManager::update()
{
	mouseDownPrev = mouseDown;
	mouseDown = App.isMouseDown(0);
	int updateIndex = -1;
	Button * but;

	for (int i = 0; i < (int)mWidgets.size(); i++)
	{
		value_t & w = mWidgets[i];
		if (strcmp(typeid(*w).name(), "class Button") == 0)
		{
			but = static_cast<Button *>(w.get());
			if (!mouseDown && mouseDownPrev && but->mouseOn())
				updateIndex = i;
		}
	}

	if (updateIndex != -1)
	{
		but = static_cast<Button *>(mWidgets[updateIndex].get());
		but->callCallback();
	}
}

void WidgetManager::add(const value_t & w)
{
	mWidgets.push_back(w);
}

void WidgetManager::clear()
{
	mWidgets.clear();
}

void WidgetManager::render() const
{
	for (const value_t & w : mWidgets)
		w->render();
}