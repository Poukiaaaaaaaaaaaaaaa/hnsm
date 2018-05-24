#include "Button.h"

#define HOVERED 1
#define NOT_HOVERED 0

Button::Button()
{

}

Button::Button(Game& g, int layerIndex, ButtonData& data)
{
	linked = &g;

	dim = data.dim;

	callbackFunction = data.callback;
	callbackData = data.cbData;

	indexa = layerIndex;
	if (!linked->layers.size() || indexa >= linked->layers.size() || indexa < 0)
	{
		linked->layers.push_back({});
		indexa = linked->layers.size() - 1;
	}

	linked->layers[indexa].push_back(GObject(
		this, { data.not_hovered, data.hovered }, true
	));

	indexb = linked->layers[indexa].size() - 1;
}

Button::~Button()
{
}

bool Button::isHovered()
{
	if (linked->mouse_x > dim.x &&
		linked->mouse_y > dim.y &&
		linked->mouse_x < dim.x + dim.w &&
		linked->mouse_y < dim.y + dim.h
		) return true;
	else return false;
}

bool Button::isClicked()
{
	if (linked->e.button.state == SDL_PRESSED &&
		linked->e.button.button == SDL_BUTTON_LEFT &&
		isHovered() && canPress == true
		)
	{
		canPress = false;
		return true;
	}
	else if (linked->e.button.state == SDL_RELEASED)
	{
		canPress = true;
	}
	return false;
}

void Button::update()
{
	if (isHovered())
	{
		linked->layers[indexa][indexb].set_current_texture(HOVERED);
	}
	else
	{
		linked->layers[indexa][indexb].set_current_texture(NOT_HOVERED);
	}
}

void Button::events()
{
	if (isClicked())
	{
		callbackFunction(callbackData);
	}
}
