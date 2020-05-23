/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cube class
-------------------------------------------------------------*/

#ifndef H_CUBE
#define H_CUBE
#include <glm/glm.hpp>
#include "SceneObject.h"
#include "Plane.h"

/**
 * Defines a simple Cube located at 'center'
 * with the specified size
 */
class Cube
{

private:
    glm::vec3 center = glm::vec3(0);
    float size = 1;
    Plane _planes[6];

public:
	Cube() {};  //Default constructor creates a unit cube

	Cube(glm::vec3 c, float s);

	Plane* planes();
};

#endif //!H_CUBE
