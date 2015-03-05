#include "MoveCommand.h"


MoveCommand::MoveCommand(glm::vec3 displacement, GameObject& actor) : Command(actor)
{
	this->displacement = displacement;
}


MoveCommand::~MoveCommand()
{}

void MoveCommand::execute()
{
	actor.transform->move(displacement);
}