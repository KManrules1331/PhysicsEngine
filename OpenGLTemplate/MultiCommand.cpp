#include "MultiCommand.h"


MultiCommand::MultiCommand()
{
}


MultiCommand::~MultiCommand()
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

void MultiCommand::addCommand(Command* c)
{
	commands.push_back(c);
}

void MultiCommand::removeCommand(Command* c)
{
	commands.erase(std::remove(std::begin(commands), std::end(commands), c), std::end(commands));
}
