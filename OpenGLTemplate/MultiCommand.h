#pragma once
#include "Command.h"
class MultiCommand :
	public Command
{
public:
	MultiCommand();
	virtual ~MultiCommand();

	void addCommand(Command* c);
	void clearCommands();

	void execute() override;

private:
	std::vector<Command*> commands;
};

