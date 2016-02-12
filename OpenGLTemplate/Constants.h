#pragma once
#define PI 3.14159f

//GLM doesn't appear to have a square function so I made this
template<typename T>
inline T square(T value){ return value * value; }

//I want to multiply vec3 by mat4 and glm doesn't allow that
//Could probably turn this into a template too but I don't feel like it rn
inline glm::vec3 transformVec3(glm::vec3 vec, glm::mat4 mat)
{
	return glm::vec3{ mat * glm::vec4{ vec, 1.0f } };
}