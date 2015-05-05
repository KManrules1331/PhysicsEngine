#include "MultiCommand.h"


MultiCommand::MultiCommand()
{
}


MultiCommand::~MultiCommand()
{
	commands.clear();
}

void MultiCommand::addCommand(Command* c)
{
	commands.push_back(c);
}

void MultiCommand::clearCommands()
{
	commands.clear();
}

void MultiCommand::execute()
{
	for (int i = 0; i < commands.size(); i++)
	{
		commands[i]->execute();
	}
}

