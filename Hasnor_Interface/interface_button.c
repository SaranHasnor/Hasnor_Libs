#include "interface_button.h"
#include "interface_render.h"

button_t *newButton(void)
{
	button_t *button = alloc(button_t);
	Memory.set(button, 0, sizeof(button_t));
	return button;
}

void destroyButton(button_t *button)
{
	if (button->text)
	{
		dealloc(button->text);
	}
	dealloc(button);
}

void drawButton(button_t *button, staticPlacement_t placement)
{
	// Border
	glColor3f(0.0f, 0.0f, 0.0f);
	drawRect(placement);

	// Text
	glColor3f(1.0f,1.0f,1.0f);
	renderString(button->dynText ? *button->dynText : button->text, placement, ANCHOR_CENTER, -1);
}