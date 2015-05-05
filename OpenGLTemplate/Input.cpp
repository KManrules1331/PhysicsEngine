#include "Input.h"

#pragma region Static Attributes

std::map<unsigned char, bool> Input::Inputs;
std::map<unsigned char, std::vector<Command*>> Input::PressBinds;
std::map<unsigned char, std::vector<Command*>> Input::ReleaseBinds;
std::map<int, bool> Input::MouseInputs;
Input::MousePosition Input::mousePosition;
std::map<int, std::vector<Command*>> Input::MousePressBinds;
std::map<int, std::vector<Command*>> Input::MouseReleaseBinds;

#pragma endregion

void Input::recordKeyboardPress(unsigned char key, int x, int y)
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
	recordMousePos(x, y);
}

void Input::recordKeyboardRelease(unsigned char key, int x, int y)
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
	recordMousePos(x, y);
}

void Input::removeKeyboardPressBind(unsigned char key, Command* c)
{
	RemoveCommand(PressBinds[key], c);
}

void Input::removeKeyboardReleaseBind(unsigned char key, Command* c)
{
	RemoveCommand(ReleaseBinds[key], c);
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

void Input::recordMouseClick(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		if (MouseInputs[button] == false)
		{
			int numCommands = MousePressBinds[button].size();
			for (int i = 0; i < numCommands; i++)
			{
				MousePressBinds[button][i]->execute();
			}
			MouseInputs[button] = true;
		}
	}
	else if (state == GLUT_UP)
	{
		if (MouseInputs[button] == true)
		{
			int numCommands = MouseReleaseBinds[button].size();
			for (int i = 0; i < numCommands; i++)
			{
				MouseReleaseBinds[button][i]->execute();
			}
			MouseInputs[button] = false;
		}
	}
	recordMousePos(x, y);
}

void Input::recordMousePos(int x, int y)
{
	mousePosition.x = x;
	mousePosition.y = y;
}

Input::MousePosition Input::getMousePos()
{
	return mousePosition;
}

void Input::removeMousePressBind(int button, Command* c)
{
	RemoveCommand(MousePressBinds[button], c);
}

void Input::removeMouseReleaseBind(int button, Command* c)
{
	RemoveCommand(MouseReleaseBinds[button], c);
}

void Input::OnMousePress(int button, Command* c)
{
	MousePressBinds[button].push_back(c);
}

void Input::OnMouseRelease(int button, Command* c)
{
	MouseReleaseBinds[button].push_back(c);
}

bool Input::ButtonPressed(int button)
{
	return MouseInputs[button];
}

void Input::ClearMousePressBinds(int button)
{
	if (button == -1)
	{
		for (std::map<int, std::vector<Command*>>::iterator i = MousePressBinds.begin(); i != MousePressBinds.end(); i++)
		{
			i->second.clear();
		}
		return;
	}
	MousePressBinds[button].clear();
}

void Input::ClearMouseReleaseBinds(int button)
{
	if (button == -1)
	{
		for (std::map<int, std::vector<Command*>>::iterator i = MouseReleaseBinds.begin(); i != MouseReleaseBinds.end(); i++)
		{
			i->second.clear();
		}
		return;
	}
	MouseReleaseBinds[button].clear();
}

void Input::RemoveCommand(std::vector<Command*>& commandList, Command* c)
{
	int numCommands = commandList.size();
	for (int i = 0; i < numCommands;)
	{
		if (commandList[i] == c)
		{
			commandList.erase(commandList.begin() + i);
			numCommands--;
		}
		else
		{
			i++;
		}
	}
}