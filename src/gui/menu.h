#pragma once

#include <string>
using std::string;

enum TypeMenu
{
	MENU_NULL,
	MENU_MAIN,
	MENU_PAUSE
};

class World;

class Menu
{
private:
	TypeMenu mType;
	World * mWorld;
public:
	Menu();
	void init(World * p);
	void change(TypeMenu no);
	void process(const string & name);
	TypeMenu getType() const;
};