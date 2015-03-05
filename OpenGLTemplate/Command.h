#pragma once
#include "GameObject.h"

class Command
{
public:
	Command(GameObject& actor);
	virtual ~Command();
	virtual void execute() = 0;
protected:
	GameObject& actor;
};

