#pragma once
#include <map>

#include "GameObject.h"
#include "CommandTypes.h"

class Input
{
public:
	static struct MousePosition {
		int x;
		int y;
	};
	//Keyboard functions
	static void recordKeyboardPress(unsigned char key, int x, int y);
	static void recordKeyboardRelease(unsigned char key, int x, int y);
	static void OnKeyboardPress(unsigned char key, Command* c);
	static void OnKeyboardRelease(unsigned char key, Command* c);
	static void removeKeyboardPressBind(unsigned char key, Command* c);
	static void removeKeyboardReleaseBind(unsigned char key, Command* c);
	static bool KeyPressed(unsigned char key);
	static void ClearKeyboardPressBinds(unsigned char key = '\0');
	static void ClearKeyboardReleaseBinds(unsigned char key = '\0');

	//Mouse Functions
	static void recordMouseClick(int button, int state, int x, int y);
	static void recordMousePos(int x, int y);
	static Input::MousePosition getMousePos();
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
	static Input::MousePosition mousePosition;

	static void RemoveCommand(std::vector<Command*>& commandList, Command* c);
};