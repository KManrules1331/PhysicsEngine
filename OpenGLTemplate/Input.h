#pragma once
#include <map>

#include "GameObject.h"
#include "CommandTypes.h"

class Input
{
public:
	static void SetControlledActor(GameObject* actor);
	static void getKeyboardPress(unsigned char key, int x, int y);
	static void getKeyboardRelease(unsigned char key, int x, int y);
	static void bindKeyboardPress(unsigned char key, Command* c);
	static void bindKeyboardRelease(unsigned char key, Command* c);

private:
	static GameObject* actor;
	static std::map<unsigned char, bool> Inputs;
	static std::map<unsigned char, Command*> PressBinds;
	static std::map<unsigned char, Command*> ReleaseBinds;
};

