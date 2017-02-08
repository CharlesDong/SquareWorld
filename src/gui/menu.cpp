#include "menu.h"
#include "main/game.h"
#include <memory>

void buttonFunc(const string & name);

Menu::Menu()
{
}

TypeMenu Menu::getType() const
{
	return mType;
}

void Menu::init(World * p)
{
	mWorld = p;
	change(MENU_MAIN);
	Button::setCallback(buttonFunc);
}

void Menu::change(TypeMenu t)
{
	using std::make_shared;

	WidgetManager & wm = App.getWidgets();
	mType = t;
	wm.clear();

	switch (t)
	{
	case MENU_NULL:
		break;
	case MENU_MAIN:
		wm.add(make_shared<Button>(ivec2(200, 330), ivec2(400, 40), "Play"));
		wm.add(make_shared<Button>(ivec2(200, 280), ivec2(400, 40), "Options"));
		wm.add(make_shared<Button>(ivec2(200, 230), ivec2(400, 40), "Quit game"));
		break;
	case MENU_PAUSE:
		wm.add(make_shared<Label>(ivec2(200, 480), ivec2(400, 40), "Game menu"));
		wm.add(make_shared<Button>(ivec2(200, 330), ivec2(400, 40), "Back to game"));
		wm.add(make_shared<Button>(ivec2(200, 280), ivec2(400, 40), "Options"));
		wm.add(make_shared<Button>(ivec2(200, 230), ivec2(400, 40), "Quit to title"));
	}
}

void Menu::process(const string & name)
{
	if (name == "Play")
	{
		App.setGameStarted(true);
		App.setGamePaused(false);
		change(MENU_NULL);
		mWorld->load();
	}
	else if (name == "Quit game")
		App.close();
	else if (name == "Back to game")
	{
		App.setGamePaused(false);
		change(MENU_NULL);
	}
	else if (name == "Quit to title")
	{
		App.setGameStarted(false);
		change(MENU_MAIN);
	}
}

void buttonFunc(const string & name)
{
	App.getMenu().process(name);
}