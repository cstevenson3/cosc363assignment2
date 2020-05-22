/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cylinder class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cylinder.h"
#include <math.h>

/**
* Cylinder's intersection method.  The input is a ray.
*/
float Cylinder::intersect(glm::vec3 p0, glm::vec3 dir)
{
	glm::vec2 p0xz = glm::vec2(p0.x, p0.z);
	glm::vec2 dirxz = glm::vec2(dir.x, dir.z);
	glm::vec2 bcxz = glm::vec2(baseCenter.x, baseCenter.z);

	glm::vec2 s = p0xz - bcxz;

	float a = glm::dot(dirxz, dirxz);
	float b = 2 * glm::dot(dirxz, s);
	float c = glm::dot(s, s) - glm::dot(radius, radius);

	float delta = b * b - 4 * a * c;

	if(delta < 0.001) {
		return -1.0;
	}

	float t1 = (-b - sqrt(delta)) / (2 * a);
	float t2 = (-b + sqrt(delta)) / (2 * a);

	if(!(t1 < 0.001) && ((p0 + glm::vec3(t1) * dir).y) > baseCenter.y && ((p0 + glm::vec3(t1) * dir).y) < (baseCenter.y + height)) {
		return t1;
	}

	if(!(t2 < 0.001) && ((p0 + glm::vec3(t2) * dir).y) > baseCenter.y && ((p0 + glm::vec3(t2) * dir).y) < (baseCenter.y + height)) {
		return t2;
	}

	return -1.0;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the sphere.
*/
glm::vec3 Cylinder::normal(glm::vec3 p)
{
    glm::vec3 n = p - baseCenter;
    n.y = 0; //assumes cylinder is upright
    n = glm::normalize(n);
    return n;
}
