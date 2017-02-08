#include "widgets.h"
#include "renderer/renderer.h"
#include "renderer/text_renderer.h"
#include "main/game.h"

Button::Callback Button::callback;

Widget::Widget(const ivec2 & p, const ivec2 & s, const string & str) : position(p), size(s), name(str)
{
}

Label::Label(const ivec2 & p, const ivec2 & s, const string & str) : Widget(p, s, str)
{
}

void Label::update()
{
}

void Label::render() const
{
	int len = TextRenderer::length(name.c_str());
	ivec2 pos = ivec2(position.x + (size.x - len) / 2, position.y + (size.y - TextRenderer::maxHeight()) / 2);
	TextRenderer::text(pos.x, pos.y, name.c_str());
}

Button::Button(const ivec2 & p, const ivec2 & s, const string & str, bool e) : Widget(p, s, str), enabled(e)
{
}

bool Button::mouseOn() const
{
	ivec2 cpos = App.getCursorPos();
	return cpos.x >= position.x && cpos.x < position.x + size.x && cpos.y >= position.y && cpos.y < position.y + size.y;
}

void Button::update()
{
}

void Button::render() const
{
	Renderer::setSize(dvec2(size));
	if (!enabled)
		Renderer::setTexture("button2");
	else if (mouseOn())
		Renderer::setTexture("button1");
	else
		Renderer::setTexture("button");
	Renderer::render(dvec2(position));
	TextRenderer::text(
		position.x + (size.x - TextRenderer::length(name.c_str())) / 2,
		position.y + (size.y - TextRenderer::maxHeight()) / 2,
		name.c_str()
		);
}

void Button::callCallback()
{
	if (enabled)
		callback(name);
}

void Button::setCallback(Callback cb)
{
	callback = cb;
}