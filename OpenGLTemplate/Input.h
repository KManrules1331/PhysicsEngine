#pragma once
#include <map>

#include "GameObject.h"
#include "CommandTypes.h"

class Input
{
public:
	static void getKeyboardPress(unsigned char key, int x, int y);
	static void getKeyboardRelease(unsigned char key, int x, int y);
	static void OnKeyboardPress(unsigned char key, Command* c);
	static void OnKeyboardRelease(unsigned char key, Command* c);
	static bool KeyPressed(unsigned char key);
	static void ClearKeyboardPressBinds(unsigned char key = '\0');
	static void ClearKeyboardReleaseBinds(unsigned char key = '\0');

private:
	static std::map<unsigned char, bool> Inputs;
	static std::map<unsigned char, Command*> PressBinds;
	static std::map<unsigned char, Command*> ReleaseBinds;
};