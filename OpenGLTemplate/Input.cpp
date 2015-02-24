#include "Input.h"

void Input::SetControlledActor(GameObject* actor)
{
	Input::actor = actor;
}
void Input::getKeyboardPress(unsigned char key, int x, int y)
{
	if (Inputs[key] == false)
	{
		if (PressBinds[key])	PressBinds[key]->execute(*(actor));
		Inputs[key] = true;
	}
}

void Input::getKeyboardRelease(unsigned char key, int x, int y)
{
	if (Inputs[key] == true)
	{
		if (ReleaseBinds[key])	ReleaseBinds[key]->execute(*(actor));
		Inputs[key] = false;
	}
}

void Input::bindKeyboardPress(unsigned char key, Command* c)
{
	delete PressBinds[key];
	PressBinds[key] = c;
}

void Input::bindKeyboardRelease(unsigned char key, Command* c)
{
	delete ReleaseBinds[key];
	ReleaseBinds[key] = c;
}