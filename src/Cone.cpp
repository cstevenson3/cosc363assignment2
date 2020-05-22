/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cylinder class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cone.h"
#include <math.h>

/**
* Cone's intersection method.  The input is a ray.
*/
float Cone::intersect(glm::vec3 p0, glm::vec3 dir)
{
	glm::vec2 p0xz = glm::vec2(p0.x, p0.z);
	glm::vec2 dirxz = glm::vec2(dir.x, dir.z);
	glm::vec2 cxz = glm::vec2(center.x, center.z);

	glm::vec2 s = p0xz - cxz;

	float a = glm::dot(dirxz, dirxz) - (radius / height) * (radius / height) * (dir.y * dir.y);
	float b = 2 * glm::dot(dirxz, s) - (radius / height) * (radius / height) * (2 * (p0.y - center.y) * dir.y);
	float c = glm::dot(s, s) - (radius / height) * (radius / height) * ((p0.y - center.y) * (p0.y - center.y));

	float delta = b * b - 4 * a * c;

	if(delta < 0.001) {
		return -1.0;
	}

	float t1 = (-b - sqrt(delta)) / (2 * a);
	float t2 = (-b + sqrt(delta)) / (2 * a);

	if(!(t1 < 0.001) && ((p0 + glm::vec3(t1) * dir).y) < center.y && ((p0 + glm::vec3(t1) * dir).y) > (center.y - height)) {
		return t1;
	}

	if(!(t2 < 0.001) && ((p0 + glm::vec3(t2) * dir).y) < center.y && ((p0 + glm::vec3(t2) * dir).y) > (center.y - height)) {
		return t2;
	}

	return -1.0;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the sphere.
*/
glm::vec3 Cone::normal(glm::vec3 p)
{
    glm::vec3 diff = p - center;
    glm::vec3 planeVec = glm::cross(diff, glm::vec3(0., 1., 0.));
    glm::vec3 n = glm::cross(planeVec, diff);
    n = glm::normalize(n);
    return n;
}
