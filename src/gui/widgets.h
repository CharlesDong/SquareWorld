#pragma once

#include "common/common.h"

enum TypeEvent
{
};

class Widget
{
protected:
	ivec2 position;
	ivec2 size;
	string name;
public:
	Widget(const ivec2 & p, const ivec2 & s, const string & str);
	virtual void update() = 0;
	virtual void render() const = 0;
};

class Label : public Widget
{
public:
	Label(const ivec2 & p, const ivec2 & s, const string & str);
	void update();
	void render() const;
};

class Button : public Widget
{
private:
	bool enabled;

	typedef void(*Callback)(const string &);
	static Callback callback;
public:
	Button(const ivec2 & p, const ivec2 & s, const string & str, bool e = true);
	bool mouseOn() const;
	void update();
	void render() const;
	void callCallback();

	static void setCallback(Callback cb);
};