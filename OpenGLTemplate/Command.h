#pragma once
#include "GameObject.h"

class Command
{
public:
	Command();
	virtual ~Command();
	virtual void execute() = 0;
};

