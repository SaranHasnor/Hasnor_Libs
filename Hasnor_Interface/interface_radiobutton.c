#include "interface_radiobutton.h"
#include "interface_render.h"

radioButton_t *newRadioButton(void)
{
	radioButton_t *radio = alloc(radioButton_t);
	Memory.set(radio, 0, sizeof(radioButton_t));
	return radio;
}

void destroyRadioButton(radioButton_t *radioButton)
{
	dealloc(radioButton);
}

void drawRadioButton(radioButton_t *rButton, staticPlacement_t placement)
{
	// Border FIXME: MAKE THIS A CIRCLE
	glColor3f(1.0f,1.0f,1.0f);
	drawRect(placement);

	if (rButton->value)
	{ // Check sign
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_LINES);
		glVertex2i(placement.x, placement.y);
		glVertex2i(placement.x+placement.w, placement.y+placement.h);

		glVertex2i(placement.x+placement.w, placement.y);
		glVertex2i(placement.x, placement.y+placement.h);
		glEnd();
	}
}