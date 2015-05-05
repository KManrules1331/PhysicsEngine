#pragma once
#include "Command.h"
class MultiCommand :
	public Command
{
public:
	MultiCommand();
	virtual ~MultiCommand();

	void execute() override;

	void addCommand(Command* c);
	void clearCommands();

private:
	std::vector<Command*> commands;
};

