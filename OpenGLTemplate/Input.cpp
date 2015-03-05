#include "Input.h"

#pragma region Static Attributes

std::map<unsigned char, bool> Input::Inputs;
std::map<unsigned char, std::vector<Command*>> Input::PressBinds;
std::map<unsigned char, std::vector<Command*>> Input::ReleaseBinds;

#pragma endregion

void Input::getKeyboardPress(unsigned char key, int x, int y)
{
	if (Inputs[key] == false)
	{
		int numCommands = PressBinds[key].size();
		for (int i = 0; i < numCommands; i++)
		{
			PressBinds[key][i]->execute();
		}
		Inputs[key] = true;
	}
}

void Input::getKeyboardRelease(unsigned char key, int x, int y)
{
	if (Inputs[key] == true)
	{
		int numCommands = ReleaseBinds[key].size();
		for (int i = 0; i < numCommands; i++)
		{
			ReleaseBinds[key][i]->execute();
		}
		Inputs[key] = false;
	}
}

void Input::removeKeyboardPressBind(unsigned char key, Command* c)
{
	std::vector<Command*>& listReference = PressBinds[key];
	int numCommands = listReference.size();
	for (int i = 0; i < numCommands;)
	{
		if (listReference[i] == c)
		{
			listReference.erase((PressBinds[key].begin() + i));
			numCommands--;
		}
		else
		{
			i++;
		}
	}
}

void Input::removeKeyboardReleaseBind(unsigned char key, Command* c)
{
	std::vector<Command*>& listReference = ReleaseBinds[key];
	int numCommands = listReference.size();
	for (int i = 0; i < numCommands;)
	{
		if (listReference[i] == c)
		{
			listReference.erase((listReference.begin() + i));
			numCommands--;
		}
		else
		{
			i++;
		}
	}
}

void Input::OnKeyboardPress(unsigned char key, Command* c)
{
	PressBinds[key].push_back(c);
}

void Input::OnKeyboardRelease(unsigned char key, Command* c)
{
	ReleaseBinds[key].push_back(c);
}

bool Input::KeyPressed(unsigned char key)
{
	return Inputs[key];
}

void Input::ClearKeyboardPressBinds(unsigned char key)
{
	if (key == '\0')
	{
		for (std::map<unsigned char, std::vector<Command*>>::iterator i = PressBinds.begin(); i != PressBinds.end(); i++)
		{
			i->second.clear();
		}
		return;
	}
	PressBinds[key].clear();
}

void Input::ClearKeyboardReleaseBinds(unsigned char key)
{
	if (key == '\0')
	{
		for (std::map<unsigned char, std::vector<Command*>>::iterator i = ReleaseBinds.begin(); i != PressBinds.end(); i++)
		{
			i->second.clear();
		}
		return;
	}
	ReleaseBinds[key].clear();
}