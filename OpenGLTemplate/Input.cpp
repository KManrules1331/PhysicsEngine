#include "Input.h"

#pragma region Static Attributes

std::map<unsigned char, bool> Input::Inputs;
std::map<unsigned char, Command*> Input::PressBinds;
std::map<unsigned char, Command*> Input::ReleaseBinds;

#pragma endregion

void Input::getKeyboardPress(unsigned char key, int x, int y)
{
	if (Inputs[key] == false)
	{
		if (PressBinds[key])	PressBinds[key]->execute();
		Inputs[key] = true;
	}
}

void Input::getKeyboardRelease(unsigned char key, int x, int y)
{
	if (Inputs[key] == true)
	{
		if (ReleaseBinds[key])	ReleaseBinds[key]->execute();
		Inputs[key] = false;
	}
}

void Input::OnKeyboardPress(unsigned char key, Command* c)
{
	delete PressBinds[key];
	PressBinds[key] = c;
}

void Input::OnKeyboardRelease(unsigned char key, Command* c)
{
	delete ReleaseBinds[key];
	ReleaseBinds[key] = c;
}

bool Input::KeyPressed(unsigned char key)
{
	return Inputs[key];
}

void Input::ClearKeyboardPressBinds(unsigned char key)
{
	if (key == '\0')
	{
		for (std::map<unsigned char, Command*>::iterator i = PressBinds.begin(); i != PressBinds.end(); i++)
		{
			delete i->second;
		}
		return;
	}
	delete PressBinds[key];
}

void Input::ClearKeyboardReleaseBinds(unsigned char key)
{
	if (key == '\0')
	{
		for (std::map<unsigned char, Command*>::iterator i = ReleaseBinds.begin(); i != PressBinds.end(); i++)
		{
			delete i->second;
		}
		return;
	}
	delete ReleaseBinds[key];
}