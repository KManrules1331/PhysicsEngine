#include "CubeCollisionDetector.h"
#include "SphereCollisionDetector.h"
#include <iostream>


CubeCollisionDetector::CubeCollisionDetector(Transform& GOTransform) : CollisionDetector(GOTransform)
{
	//This initializes the local verts and edges so they don't have to be regenerated all the time
	createCube();
}


CubeCollisionDetector::~CubeCollisionDetector()
{
}

bool CubeCollisionDetector::detectCollision(CollisionDetector& c)
{
	//TODO//Implement AABB again
	//I believe there was a problem with the expression "factor = sqrt(3) / 2;" casting maybe...
	return c.detectCubeCollision(*this);
}

bool CubeCollisionDetector::detectSphereCollision(SphereCollisionDetector& c)
{
	//Implement logic here
	return c.detectCubeCollision(*this);
}

bool CubeCollisionDetector::detectCubeCollision(CubeCollisionDetector& c)
{
	std::vector<glm::vec3> vertices = getVertices();
	std::vector<glm::vec3> otherVertices = c.getVertices();
	return GJKCollisionDetection::areColliding(vertices, otherVertices);
}

CollisionDetector::ContainingBox CubeCollisionDetector::getAABB()
{
	ContainingBox returnBox;
	float factor = sqrt(3) / 2;
	glm::vec3 center = GOTransform.getPosition();
	glm::vec3 scale = GOTransform.getScale();
	returnBox.right = center.x + scale.x * factor;
	returnBox.left = center.x - scale.x * factor;
	returnBox.up = center.y + scale.y * factor;
	returnBox.down = center.y - scale.y * factor;
	returnBox.front = center.z - scale.z * factor;
	returnBox.back = center.z + scale.z * factor;

	return returnBox;
}

std::vector < glm::vec3> CubeCollisionDetector::getVertices() const
{
	//Code specific to the cube collider
	std::vector<glm::vec3> returnVector;
	returnVector.push_back(glm::vec3(-0.5f, -0.5f, -0.5f));
	returnVector.push_back(glm::vec3(-0.5f, -0.5f, 0.5f));
	returnVector.push_back(glm::vec3(-0.5f, 0.5f, -0.5f));
	returnVector.push_back(glm::vec3(-0.5f, 0.5f, 0.5f));
	returnVector.push_back(glm::vec3(0.5f, -0.5f, -0.5f));
	returnVector.push_back(glm::vec3(0.5f, -0.5f, 0.5f));
	returnVector.push_back(glm::vec3(0.5f, 0.5f, -0.5f));
	returnVector.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
	for (unsigned int i = 0; i < returnVector.size(); i++)
	{
		glm::vec4 multiplyVector = glm::vec4(returnVector[i], 1.0f);
		multiplyVector = GOTransform.transformMatrix * multiplyVector;
		returnVector[i] = glm::vec3(multiplyVector);
	}

	return returnVector;
}
//Wat is this code?
std::vector<std::vector<glm::vec3>> CubeCollisionDetector::getEdges() const
{
	std::vector<glm::vec3> vertices = getVertices();
	std::vector<std::vector<glm::vec3>> returnVector;
	//Edge 0-1
	addEdgeToList(&returnVector, &vertices, 0, 1);
	//Edge 0-2
	addEdgeToList(&returnVector, &vertices, 0, 2);
	//Edge 0-4
	addEdgeToList(&returnVector, &vertices, 0, 4);
	//Edge 1-3
	addEdgeToList(&returnVector, &vertices, 1, 3);
	//Edge 1-5
	addEdgeToList(&returnVector, &vertices, 1, 5);
	//Edge 2-3
	addEdgeToList(&returnVector, &vertices, 2, 3);
	//Edge 2-6
	addEdgeToList(&returnVector, &vertices, 2, 6);
	//Edge 3-7
	addEdgeToList(&returnVector, &vertices, 3, 7);
	//Edge 4-5
	addEdgeToList(&returnVector, &vertices, 4, 5);
	//Edge 4-6
	addEdgeToList(&returnVector, &vertices, 4, 6);
	//Edge 5-7
	addEdgeToList(&returnVector, &vertices, 5, 7);
	//Edge 6-7
	addEdgeToList(&returnVector, &vertices, 6, 7);

	return returnVector;
}
//Does not work for nonuniform scale factors//
//Returns a vector of face normals in world coordinates
std::vector<glm::vec3> CubeCollisionDetector::getNormals() const
{
	std::vector<glm::vec3> returnVector;
	returnVector.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	returnVector.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	returnVector.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	returnVector.push_back(glm::vec3(0.0f, -1.0f, 0.0f));
	returnVector.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
	returnVector.push_back(glm::vec3(0.0f, 0.0f, -1.0f));
	for (unsigned int i = 0; i < returnVector.size(); i++)
	{
		glm::vec4 multiplyVector = glm::vec4(returnVector[i], 1.0f);
		multiplyVector = GOTransform.rotationMatrix * multiplyVector;
		returnVector[i] = glm::vec3(multiplyVector);
	}

	return returnVector;
}

void CubeCollisionDetector::addEdgeToList(std::vector<std::vector<glm::vec3>>* list, std::vector<glm::vec3>* points, int pointNum1, int pointNum2) const
{
	(*list).push_back(std::vector<glm::vec3>());
	int edgeNum = (*list).size() - 1;
	(*list)[edgeNum].push_back((*points)[pointNum1]);
	(*list)[edgeNum].push_back((*points)[pointNum2]);
}

float CubeCollisionDetector::getHalfsize() const{ return halfsize; }

glm::vec3 CubeCollisionDetector::getAngularAcceleration(glm::vec3 torque, float inverseMass) const 
{
	//Because this is strictly limited to cubes I am optimizing to eliminate the use of an inertia tensor
	//The moment of inertia of a cube happens to be the same about all axes
	if (inverseMass == 0)
		return glm::vec3();
	float sideLength = halfsize * 2.0f;
	return torque * (sideLength * sideLength) / (inverseMass * 6.0f);
}

bool CubeCollisionDetector::getCollisionInfo(CollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact)
{
	return c.getCubeCollisionInfo(*this, pointOfContact, normalOfContact);
}


bool CubeCollisionDetector::getCubeCollisionInfo(CubeCollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact)
{
	//First, point of contact
		//Check for vertex in Mesh
	std::vector<glm::vec3> vertices = getVertices();
	std::vector<glm::vec3> collidingVertices;
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		std::vector<glm::vec3> fakeVertexList;
		fakeVertexList.push_back(vertices[i]);
		if (GJKCollisionDetection::areColliding(c.getVertices(), fakeVertexList))
		{
			collidingVertices.push_back(vertices[i]);
		}
	}
	if (collidingVertices.size() > 0)
	{
		//We have vertex in mesh, we can derive point of contact
		*pointOfContact = glm::vec3();
		for (unsigned int i = 0; i < collidingVertices.size(); i++)
		{
			*pointOfContact += collidingVertices[i];
		}
		*pointOfContact /= collidingVertices.size();

		glm::vec3 diff = *pointOfContact - c.GOTransform.getPosition();
		std::vector<glm::vec3> normals = c.getNormals();
		int index;
		float max = 0;
		for (unsigned int i = 0; i < normals.size(); i++)
		{
			float weight = glm::dot(diff, normals[i]);
			if (weight > max)
			{
				max = weight;
				index = i;
			}
		}

		*normalOfContact = normals[index];
	}
	else
	{
		//Check for edge on edge
		std::vector<std::vector<glm::vec3>> edges = getEdges();
		int numEdgesColliding = 0;
		for (unsigned int i = 0; i < edges.size(); i++)
		{
			if (GJKCollisionDetection::areColliding(edges[i], c.getVertices()))
			{
				for (unsigned int j = 0; j < edges[i].size(); j++)
				{
					*pointOfContact += edges[i][j];
				}
				numEdgesColliding += 2;
			}
		}
		*pointOfContact /= numEdgesColliding;

		if (numEdgesColliding > 0)
		{
			std::vector<std::vector<glm::vec3>> otherEdges = c.getEdges();
			int numEdgesColliding2 = 0;
			glm::vec3 normal = glm::vec3();
			for (unsigned int i = 0; i < otherEdges.size(); i++)
			{
				if (GJKCollisionDetection::areColliding(otherEdges[i], getVertices()))
				{
					for (unsigned int j = 0; j < otherEdges[i].size(); j++)
					{
						normal += otherEdges[i][j] - c.GOTransform.getPosition();
					}
					numEdgesColliding2 += 2;
				}
			}
			normal /= numEdgesColliding2;

			*normalOfContact = normal;
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool CubeCollisionDetector::getSphereCollisionInfo(SphereCollisionDetector& c, glm::vec3* pointOfContact, glm::vec3* normalOfContact)
{
	return false;
}

//New SAT dispatch track///////////////////////////////////////////////////////////////////////////
bool CubeCollisionDetector::getSATCollisionInfo(CollisionDetector& c, Contact* contact)
{
	return c.getSATCollisionInfo(*this, contact);
}
bool CubeCollisionDetector::getSATCollisionInfo(SphereCollisionDetector& c, Contact* contact)
{
	return c.getSATCollisionInfo(*this, contact);
}

//TODO//Modify so that contact always points at "this"
//Dispatch track is working properly and this method is being reached
//Entry point for collision resolution//Should be done here as long as getFaceContact methods work
bool CubeCollisionDetector::getSATCollisionInfo(CubeCollisionDetector& b, Contact* contact)
{
	//Code is from the old "handleCollision method"	
	CubeCollisionDetector& a = *this;

	Contact aFaceContact, bFaceContact, edgeContact, tempContact;
	tempContact.depth = FLT_MAX;

	//Get min contact from a axes//FACE contact
	aFaceContact = getFaceContact(a, b);
	if (aFaceContact.depth < tempContact.depth)
		tempContact = aFaceContact;

	//Get min contact from b axes//FACE contact
	bFaceContact = getFaceContact(b, a);
	if (bFaceContact.depth < tempContact.depth)
		tempContact = bFaceContact;

	//TODO//Fix edge contact
	//Test all edges axes for minOverlap/EDGE contact
	edgeContact = getEdgeContact(a, b);
	if (edgeContact.depth < tempContact.depth)
		;// tempContact = edgeContact;

	//Test if either contact is negative depth
	if (tempContact.depth <= 0)
		return false;

	//Make sure the contact always points at a
	if (glm::dot(tempContact.normal, a.GOTransform.getPosition() - b.GOTransform.getPosition()) < 0)
		tempContact.normal = -tempContact.normal;

	//Modify the passed in contact and return 
	*contact = tempContact;
	return true;
}

//Everything from here down is imported from my code
//Trying to have all methods work as they did before and just use the new API to get data

//DONE//
//These are (or should be) indexed in order: e1 e2 e3
//Returns vectors parallel to edges in WORLD coords//Modified to use new code
std::vector<glm::vec3> CubeCollisionDetector::getWorldEdgeAxes() const
{
	//Update model matrix
	glm::mat4 model = GOTransform.transformMatrix;
	std::vector<glm::vec3> temp;
	for (const glm::vec3& axis : edgeAxes)
		temp.push_back(glm::vec3{ model * glm::vec4{ axis, 1.0f } });
	return temp;
}

//DONE//
//Don't know what I even used this for...
//Returns in model coordinates//Just a vector containing e1, e2, e3
std::vector<glm::vec3> CubeCollisionDetector::getModelEdgeAxes() const{ return edgeAxes; }

//NOT DONE//
//Returns contact from two objects//Checks all face to vertex contacts
//Separating axes are taken from first parameter
//Returns a default contact if there is no intersection
Contact CubeCollisionDetector::getFaceContact(const CubeCollisionDetector& a, const CubeCollisionDetector& b)
{
	Contact contact;
	float minOverlap = FLT_MAX;
	int vertIndex = 0;
	int axisIndex;

	//Determine axis with lowest overlap
	//Wondering why it is allowing this method to be called when it's private
	auto axes = a.getNormals();
	for (unsigned int i = 0; i < axes.size(); i++)
	{
		float overlap = overlapAmount(axes[i], a, b, &vertIndex);
		if (overlap < minOverlap)
		{
			minOverlap = overlap;
			axisIndex = i;
		}
	}
	if (minOverlap <= 0)
		return Contact{};

	//Determine contact normal
	contact.normal = axes[axisIndex];

	//Determine point of contact//Will be a vertex from b//Get index from minMaxProjection method
	contact.position = b.getWorldVert(vertIndex);

	//Determine depth of contact
	contact.depth = minOverlap;

	return contact;
}

//NOT DONE//
//Returns a filled contact data structure if given edges and colliders to draw vertices from 
Contact CubeCollisionDetector::getEdgeContact(const CubeCollisionDetector& a, const CubeCollisionDetector& b)
{
	Contact contact;
	std::vector<glm::vec3> axes;
	int aAxisIndex;
	int bAxisIndex;

	//Generate list of axes using cross products of the transform base axes
	for (const glm::vec3& axis1 : a.getWorldEdgeAxes())
		for (const glm::vec3& axis2 : b.getWorldEdgeAxes())
			axes.push_back(glm::normalize(glm::cross(axis1, axis2)));

	//Determine axis with lowest overlap
	float minOverlap = FLT_MAX;
	for (unsigned int i = 0; i < axes.size(); i++)
	{
		float overlap = overlapAmount(axes[i], a, b);
		if (overlap < minOverlap)
		{
			minOverlap = overlap;
			aAxisIndex = i / 3;
			bAxisIndex = i % 3;
			contact.normal = axes[i];
		}
	}

	//If(negative overlap) exit
	if (minOverlap < 0)
		return Contact{};

	//Determine which edges are in contact
	//Get edge points in object coords
	std::vector<glm::vec3> aPoints = a.getEdgePoints(aAxisIndex);
	std::vector<glm::vec3> bPoints = b.getEdgePoints(bAxisIndex);

	//Transform to world coords
	for (unsigned int i = 0; i < aPoints.size(); i++)
		aPoints[i] = glm::vec3{ a.GOTransform.transformMatrix * glm::vec4{ aPoints[i], 1.0f } };
	for (unsigned int i = 0; i < bPoints.size(); i++)
		bPoints[i] = glm::vec3{ b.GOTransform.transformMatrix * glm::vec4{ bPoints[i], 1.0f } };

	//Determine which in each set is closest to other object
	glm::vec3 point1 = findClosest(aPoints, b.GOTransform.getPosition());
	glm::vec3 point2 = findClosest(bPoints, a.GOTransform.getPosition());

	//Determine point and depth of contact (use helper method above from Millington book)
	contact.position = getEdgeContactPoint(a.getWorldEdgeAxes()[aAxisIndex], b.getWorldEdgeAxes()[bAxisIndex], point1, point2, &(contact.depth));

	return contact;
}

//DONE?//
//Should work without modification
//Determines the contact point of an edge to edge collision if given two edges defined by a point and a direction
glm::vec3 CubeCollisionDetector::getEdgeContactPoint(const glm::vec3& axisOne, const glm::vec3& axisTwo, const glm::vec3& ptOnEdgeOne, const glm::vec3& ptOnEdgeTwo, float* depth)
{
	glm::vec3 toSt = ptOnEdgeOne - ptOnEdgeTwo;
	float dpStaOne = glm::dot(axisOne, toSt);
	float dpStaTwo = glm::dot(axisTwo, toSt);

	float smOne = glm::dot(axisOne, axisOne);
	float smTwo = glm::dot(axisTwo, axisTwo);
	float dotProductEdges = glm::dot(axisTwo, axisOne);
	float denom = glm::dot(smOne, smTwo) - glm::dot(dotProductEdges, dotProductEdges);
	float a = (glm::dot(dotProductEdges, dpStaTwo) - glm::dot(smTwo, dpStaOne)) / denom;
	float b = (glm::dot(smOne, dpStaTwo) - glm::dot(dotProductEdges, dpStaOne)) / denom;

	glm::vec3 nearestPtOne = ptOnEdgeOne + axisOne * a;
	glm::vec3 nearestPtTwo = ptOnEdgeTwo + axisTwo * b;
	*depth = glm::distance(nearestPtOne, nearestPtTwo);
	return nearestPtOne * 0.5f + nearestPtTwo * 0.5f;
}

//DONE?//
//This should work without modification
//Determine how much two shapes are overlapping on a single separating axis
float CubeCollisionDetector::overlapAmount(glm::vec3 axis, const CubeCollisionDetector& a, const CubeCollisionDetector& b)
{
	float overlap;

	//Project verts from this onto axis
	float minA = FLT_MAX;
	float maxA = -FLT_MAX;
	int aMaxIndex = 0;
	int aMinIndex = 0;
	a.getMinMaxProjections(axis, &minA, &maxA, &aMinIndex, &aMaxIndex);

	//Project verts from a onto axis
	float minB = FLT_MAX;
	float maxB = -FLT_MAX;
	int bMaxIndex = 0;
	int bMinIndex = 0;
	b.getMinMaxProjections(axis, &minB, &maxB, &bMinIndex, &bMaxIndex);

	//This will fail on an axis where on object is entirely inside another
	if (minA < minB)
	{
		overlap = maxA - minB;
	}
	else
	{
		overlap = maxB - minA;
	}

	return overlap;
}

//DONE?//
//Determine how much two shapes are overlapping on a single separating axis and store the index of the intersecting vertex
float CubeCollisionDetector::overlapAmount(glm::vec3 axis, const CubeCollisionDetector& a, const CubeCollisionDetector& b, int* vertIndex)
{
	float overlap;

	//Project verts from a onto axis
	float minB = FLT_MAX;
	float maxB = -FLT_MAX;
	int aMaxIndex = 0;
	int aMinIndex = 0;
	a.getMinMaxProjections(axis, &minB, &maxB, &aMinIndex, &aMaxIndex);

	//Project verts from b onto axis
	float minA = FLT_MAX;
	float maxA = -FLT_MAX;

	int bMaxIndex = 0;
	int bMinIndex = 0;
	b.getMinMaxProjections(axis, &minA, &maxA, &bMinIndex, &bMaxIndex);

	if (minA < minB)
	{
		*vertIndex = bMinIndex;
		overlap = maxA - minB;
	}
	else
	{
		*vertIndex = bMaxIndex;
		overlap = maxB - minA;
	}
	return overlap;
}

//DONE?//
//Gets the min and max projections of the vertices of this object onto the axis given
//Also finds the indices of the min and max vertices
void CubeCollisionDetector::getMinMaxProjections(const glm::vec3 axis, float* min, float* max, int* minIndex, int* maxIndex) const
{
	auto worldVerts = getVertices();
	for (unsigned int i = 0; i < numVerts; i++)
	{
		float projection = glm::dot(worldVerts[i], axis);
		if (projection < *min)
		{
			*minIndex = i;
			*min = projection;
		}
		if (projection > *max)
		{
			*maxIndex = i;
			*max = projection;
		}
	}
	return;
}

//Should work without modification
//Returns the closest vector in the set to the given vector
glm::vec3 CubeCollisionDetector::findClosest(std::vector<glm::vec3> set, glm::vec3 point)
{
	glm::vec3 closest;
	float minDistance = FLT_MAX;

	//Uses square distance for faster computation
	for (const glm::vec3& testPoint : set)
	{
		glm::vec3 x = point - testPoint;
		float dist = glm::dot(x, x);
		if (dist < minDistance)
		{
			minDistance = dist;
			closest = testPoint;
		}
	}
	return closest;
}

//Get the 4 points on the edges of the parallel edges specified by the index argument
std::vector<glm::vec3> CubeCollisionDetector::getEdgePoints(int index) const
{
	std::vector<glm::vec3> set;
	glm::vec3 basePoint = glm::vec3{ 1.0f, 1.0f, 1.0f } -getWorldEdgeAxis(index);
	glm::vec3 p1, p2;
	switch (index)
	{
	case 0: p1 = glm::vec3{ 0, 0, 2.0f }; p2 = glm::vec3(0, 2.0f, 0); break;
	case 1: p1 = glm::vec3{ 0, 0, 2.0f }; p2 = glm::vec3(2.0f, 0, 0); break;
	case 2: p1 = glm::vec3{ 2.0f, 0, 0 }; p2 = glm::vec3(0, 2.0f, 0); break;
	}
	set.push_back(basePoint);
	set.push_back(basePoint - p1);
	set.push_back(basePoint - p2);
	set.push_back(basePoint - p2 - p1);
	return set;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

//This is called from the constructor to do some initialization stuff
void CubeCollisionDetector::createCube()
{
	//Populate list of axes
	numVerts = 8;

	//Create list of vertices in local coordinates
	verts.push_back(glm::vec3(-0.5f, -0.5f, -0.5f));
	verts.push_back(glm::vec3(-0.5f, -0.5f, 0.5f));
	verts.push_back(glm::vec3(-0.5f, 0.5f, -0.5f));
	verts.push_back(glm::vec3(-0.5f, 0.5f, 0.5f));
	verts.push_back(glm::vec3(0.5f, -0.5f, -0.5f));
	verts.push_back(glm::vec3(0.5f, -0.5f, 0.5f));
	verts.push_back(glm::vec3(0.5f, 0.5f, -0.5f));
	verts.push_back(glm::vec3(0.5f, 0.5f, 0.5f));

	//Create list of axes parallel to the edges
	edgeAxes.push_back(glm::vec3(1.0f, 0, 0));
	edgeAxes.push_back(glm::vec3(0, 1.0f, 0));
	edgeAxes.push_back(glm::vec3(0, 0, 1.0f));
}
//Get a list of all verts in world coordinates
std::vector<glm::vec3> CubeCollisionDetector::getWorldVerts() const
{
	std::vector<glm::vec3> temp;
	for (unsigned int i = 0; i < verts.size(); i++)
	{
		glm::vec4 multiplyVector = glm::vec4(verts[i], 1.0f);
		multiplyVector = GOTransform.transformMatrix * multiplyVector;
		temp[i] = glm::vec3(multiplyVector);
	}
	return temp;
}
//Get a single vertex in world coordinates
glm::vec3 CubeCollisionDetector::getWorldVert(GLuint i) const
{
	//This doesn't handle errors properly but meh
	if (i >= verts.size())
		return glm::vec3();
	return verts[i];
}
//Returns a single edge from the vector//TODO//Fix to include range checking of some kind
glm::vec3 CubeCollisionDetector::getWorldEdgeAxis(GLuint index) const
{
	glm::vec4 multiplyVector = glm::vec4(edgeAxes[index], 1.0f);
	multiplyVector = GOTransform.transformMatrix * multiplyVector;
	return glm::vec3(multiplyVector);
}