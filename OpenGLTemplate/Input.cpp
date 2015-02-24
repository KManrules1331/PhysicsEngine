#include "Input.h"

void Input::SetControlledActor(GameObject* actor)
{
	Input::actor = actor;
}
void Input::getKeyboardInput(unsigned char key, int x, int y)
{
	if (Inputs[key] = false)
	{
		PressBinds[key]->execute(*(actor));
		Inputs[key] = true;
	}
}

void Input::getKeyboardUpInput(unsigned char key, int x, int y)
{
	Inputs[key] = false;
}

void Input::bindKeyboardEvent(unsigned char key, Command* c)
{
	PressBinds[key] = c;
}