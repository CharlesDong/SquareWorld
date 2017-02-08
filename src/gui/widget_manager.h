#pragma once

#include <vector>
#include <memory>
#include "widgets.h"

class WidgetManager
{
private:
	typedef std::shared_ptr<Widget> value_t;
	std::vector<value_t> mWidgets;
	bool mouseDown = false;
	bool mouseDownPrev = false;
public:
	WidgetManager();
	void update();
	void add(const value_t & w);
	void clear();
	void render() const;
};