#pragma once
#include <map>

#include "GameObject.h"
#include "CommandTypes.h"

class Input
{
public:
	//Keyboard functions
	static void getKeyboardPress(unsigned char key, int x, int y);
	static void getKeyboardRelease(unsigned char key, int x, int y);
	static void OnKeyboardPress(unsigned char key, Command* c);
	static void OnKeyboardRelease(unsigned char key, Command* c);
	static void removeKeyboardPressBind(unsigned char key, Command* c);
	static void removeKeyboardReleaseBind(unsigned char key, Command* c);
	static bool KeyPressed(unsigned char key);
	static void ClearKeyboardPressBinds(unsigned char key = '\0');
	static void ClearKeyboardReleaseBinds(unsigned char key = '\0');

	//Mouse Functions
	static void getMouseClick(int button, int state, int x, int y);
	static void OnMousePress(int button, Command* c);
	static void OnMouseRelease(int button, Command* c);
	static void removeMousePressBind(int button, Command* c);
	static void removeMouseReleaseBind(int button, Command* c);
	static bool ButtonPressed(int button);
	static void ClearMousePressBinds(int button = -1);
	static void ClearMouseReleaseBinds(int button = -1);

private:
	static std::map<unsigned char, bool> Inputs;
	static std::map<int, bool> MouseInputs;
	static std::map<unsigned char, std::vector<Command*>> PressBinds;
	static std::map<int, std::vector<Command*>> MousePressBinds;
	static std::map<unsigned char, std::vector<Command*>> ReleaseBinds;
	static std::map<int, std::vector<Command*>> MouseReleaseBinds;
};