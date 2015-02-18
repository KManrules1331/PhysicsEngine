#include "GJKCollisionDetection.h"

bool GJKCollisionDetection::Colliding(CubeCollider* a, CubeCollider* b)
{
	glm::vec3* aVertices = getVertices(a);
	glm::vec3* bVertices = getVertices(b);
}

glm::vec3 GJKCollisionDetection::Support(glm::vec3 direction)
{

}

glm::vec3* GJKCollisionDetection::getVertices(CubeCollider* cube)
{
	glm::vec3* vertices = new glm::vec3[6];
	glm::vec3 radianAngles = *(cube->rotation) * (PI / 180);
	glm::vec3 cosAngles = glm::vec3(cos(radianAngles.x), cos(radianAngles.y), cos(radianAngles.z));
	glm::vec3 sinAngles = glm::vec3(sin(radianAngles.x), cos(radianAngles.y), cos(radianAngles.z));

	glm::mat4 rotationMatrix = glm::mat4(cosAngles.z, -sinAngles.z, 0.0f, 0.0f,
										 sinAngles.z, cosAngles.z, 0.0f, 0.0f,
										 0.0f, 0.0f, 1.0f, 0.0f,
										 0.0f, 0.0f, 0.0f, 1.0f)
							   * 

							   glm::mat4(cosAngles.y, 0.0f, -sinAngles.y, 0.0f,
										 0.0f, 1.0f, 0.0f, 0.0f,
										 sinAngles.y, 0.0f, cosAngles.y, 0.0f,
										 0.0f, 0.0f, 0.0f, 1.0f)
							   *

							   glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
										 0.0f, cosAngles.x, sinAngles.x, 0.0f,
										 0.0f, -sinAngles.x, cosAngles.x, 0.0f,
										 0.0f, 0.0f, 0.0f, 1.0f);
}