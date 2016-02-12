#include "MoveCommand.h"


MoveCommand::MoveCommand(glm::vec3 displacement, GameObject& actor) : Command(), actor{ actor }
{
	this->displacement = displacement;
}


MoveCommand::~MoveCommand()
{}

void MoveCommand::execute()
{
	actor.getTransform().move(displacement);
}
