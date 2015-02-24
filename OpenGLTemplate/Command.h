#pragma once
#include "GameObject.h"

class Command
{
public:
	virtual ~Command();
	virtual void execute(GameObject& actor) = 0;
};

