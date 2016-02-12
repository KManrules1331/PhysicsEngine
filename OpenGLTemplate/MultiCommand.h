#pragma once
#include "Command.h"
class MultiCommand :
	public Command
{
public:
	MultiCommand();
	~MultiCommand();

	void execute() override;

	void addCommand(Command* c);
	void removeCommand(Command* c);
private:
	std::vector<Command*> commands;
};
