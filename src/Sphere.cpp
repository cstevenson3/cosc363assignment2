/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The sphere class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Sphere.h"
#include <math.h>

/**
* Sphere's intersection method.  The input is a ray. 
*/
float Sphere::intersect(glm::vec3 p0, glm::vec3 dir)
{
    glm::vec3 vdif = p0 - center;   //Vector s (see Slide 28)
    float b = glm::dot(dir, vdif);
    float len = glm::length(vdif);
    float c = len*len - radius*radius;
    float delta = b*b - c;
   
	if(fabs(delta) < 0.001) return -1.0; 
    if(delta < 0.0) return -1.0;

    float t1 = -b - sqrt(delta);
    float t2 = -b + sqrt(delta);
    if(fabs(t1) < 0.001 )
    {
        if (t2 > 0) return t2;
        else t1 = -1.0;
    }
    if(fabs(t2) < 0.001 ) t2 = -1.0;

	return (t1 < t2)? t1: t2;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the sphere.
*/
glm::vec3 Sphere::normal(glm::vec3 p)
{
    glm::vec3 n = p - center;
    n = glm::normalize(n);
    return n;
}

/**
 * The st texture coords at point p
 */
glm::vec2 Sphere::stCoords(glm::vec3 p) {
	glm::vec3 dir = p - center;
	float t = dir.y / (2 * radius) + 0.5;
	float s = asin(dir.x / sqrt((radius * radius) - (dir.y * dir.y)));
	if(dir.z < 0) {
		if(dir.x > 0) {
			s = 3.141 - s;
		} else {
			s = -3.141 - s;
		}
	}
	s = s / (2 * 3.141) + 0.5;
	return glm::vec2(s, t);
}
