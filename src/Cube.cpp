/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cube class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cube.h"
#include <math.h>

Cube::Cube(glm::vec3 c, float s) {
	center = c;
	size = s;

	//front
	Plane front = Plane (glm::vec3(center.x - size/2, center.y - size / 2, center.z + size/2),
			glm::vec3(center.x + size/2, center.y - size / 2, center.z + size/2),
			glm::vec3(center.x + size/2, center.y + size / 2, center.z + size/2),
			glm::vec3(center.x - size/2, center.y + size / 2, center.z + size/2));

	//back
	Plane back = Plane(glm::vec3(center.x - size/2, center.y - size / 2, center.z - size/2),
			glm::vec3(center.x + size/2, center.y - size / 2, center.z - size/2),
			glm::vec3(center.x + size/2, center.y + size / 2, center.z - size/2),
			glm::vec3(center.x - size/2, center.y + size / 2, center.z - size/2));

	//left
	Plane left = Plane(glm::vec3(center.x - size/2, center.y - size / 2, center.z + size/2),
			glm::vec3(center.x - size/2, center.y - size / 2, center.z - size/2),
			glm::vec3(center.x - size/2, center.y + size / 2, center.z - size/2),
			glm::vec3(center.x - size/2, center.y + size / 2, center.z + size/2));

	//right
	Plane right = Plane(glm::vec3(center.x + size/2, center.y - size / 2, center.z + size/2),
			glm::vec3(center.x + size/2, center.y - size / 2, center.z - size/2),
			glm::vec3(center.x + size/2, center.y + size / 2, center.z - size/2),
			glm::vec3(center.x + size/2, center.y + size / 2, center.z + size/2));

	//bottom
	Plane bottom = Plane(glm::vec3(center.x - size/2, center.y - size / 2, center.z + size/2),
			glm::vec3(center.x + size/2, center.y - size / 2, center.z + size/2),
			glm::vec3(center.x + size/2, center.y - size / 2, center.z - size/2),
			glm::vec3(center.x - size/2, center.y - size / 2, center.z - size/2));

	//top
	Plane top = Plane(glm::vec3(center.x - size/2, center.y + size / 2, center.z + size/2),
			glm::vec3(center.x + size/2, center.y + size / 2, center.z + size/2),
			glm::vec3(center.x + size/2, center.y + size / 2, center.z - size/2),
			glm::vec3(center.x - size/2, center.y + size / 2, center.z - size/2));

	_planes[0] = front;
	_planes[1] = back;
	_planes[2] = left;
	_planes[3] = right;
	_planes[4] = bottom;
	_planes[5] = top;
}

Plane* Cube::planes() {
	return _planes;
}
