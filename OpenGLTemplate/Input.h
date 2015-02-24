#pragma once
#include <map>

#include "GameObject.h"
#include "CommandTypes.h"

class Input
{
public:
	static void SetControlledActor(GameObject* actor);
	static void getKeyboardInput(unsigned char key, int x, int y);
	static void getKeyboardUpInput(unsigned char key, int x, int y);
	static void bindKeyboardEvent(unsigned char key, Command* c);

private:
	static GameObject* actor;
	static std::map<unsigned char, bool> Inputs;
	static std::map<unsigned char, Command*> PressBinds;
};

