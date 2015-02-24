#include "MoveCommand.h"


MoveCommand::MoveCommand(glm::vec3 displacement)
{
	this->displacement = displacement;
}


MoveCommand::~MoveCommand()
{}

void MoveCommand::execute(GameObject& actor)
{
	actor.addDisplacement(displacement);
}