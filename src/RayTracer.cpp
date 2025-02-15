/*==================================================================================
* COSC 363  Computer Graphics (2020)
* Department of Computer Science and Software Engineering, University of Canterbury.
*
* A basic ray tracer
* See Lab07.pdf, Lab08.pdf for details.
*===================================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Plane.h"
#include "Cube.h"
#include "SceneObject.h"
#include "Ray.h"
#include "TextureBMP.h"
#include <GL/freeglut.h>
using namespace std;

const bool AA = true;

const float WIDTH = 40.0;
const float HEIGHT = 40.0;
const float EDIST = 40.0;
const int NUMDIV = 650;
const int MAX_STEPS = 5;
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;

vector<SceneObject*> sceneObjects;
TextureBMP texture;
TextureBMP earthTexture;

//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
	glm::vec3 backgroundCol(0);						//Background colour = (0,0,0)
	glm::vec3 lightPos(-30, 50, 0);					//Light's position
	glm::vec3 lightDir(0.3, 0, -1);
	float lightAngle = 1; // radians
	glm::vec3 color(0);
	SceneObject* obj;

	float ambient = 0.2;

    ray.closestPt(sceneObjects);					//Compare the ray with all objects in the scene
    if(ray.index == -1) return backgroundCol;		//no intersection
	obj = sceneObjects[ray.index];					//object on which the closest point of intersection is found

	if (ray.index == 0)
	{
		 //checker pattern
		 int stripeWidth = 5;
		 int ix = (ray.hit.x) / stripeWidth;
		 int iz = (ray.hit.z) / stripeWidth;
		 int k = (iz + ix) % 2; //2 colors
		 if (k == 0) color = glm::vec3(0, 1, 0);
		 else color = glm::vec3(1, 1, 0.5);
		 obj->setColor(color);

		float texcoords = (ray.hit.x - -15)/(15 - -15);
		float texcoordt = (ray.hit.z - -40)/(-70 - -40);
		if(texcoords > 0 && texcoords < 1 && texcoordt > 0 && texcoordt < 1)
		{
			color = texture.getColorAt(texcoords, texcoordt);
			obj->setColor(color);
		}
	}

	if(ray.index == 1) {
		Sphere* sphere = (Sphere*) obj;
		//Add code for texture mapping here
		float texcoords = (sphere->stCoords(ray.hit)).x;
		float texcoordt = (sphere->stCoords(ray.hit)).y;
		if(texcoords > 0 && texcoords < 1 && texcoordt > 0 && texcoordt < 1)
		{
			color = earthTexture.getColorAt(texcoords, texcoordt);
			obj->setColor(color);
		}
	}

	if(ray.index == 2) {
		float texcoords = (ray.hit.x - 12)/(22 - 12);
		float texcoordt = (ray.hit.y - -10)/(0 - -10);

		float a = texcoords * 3 - 2;
		float b = texcoordt * 2 - 1;

		//Mandelbrot set
		glm::vec2 c = glm::vec2(a, b);
		glm::vec2 z = glm::vec2(0, 0);
		bool bounded = true;
		int iterations = 0;
		for(int i = 0; i < 100; i++) {
			glm::vec2 temp = glm::vec2(0, 0);
			temp.x = z.x;
			temp.y = z.y;
			z.x = temp.x * temp.x - temp.y * temp.y + c.x;
			z.y = 2 * temp.x * temp.y + c.y;
			if(glm::length(z) > 2) {
				bounded = false;
				iterations = i;
				break;
			}
		}
		if(bounded) {
			obj->setColor(glm::vec3(0., 0., 0.));
		} else {
			obj->setColor(glm::vec3(2 * iterations / 100.) * glm::vec3(0., 1., 0.) + glm::vec3(0., 0., 1.));
		}
	}

	if(glm::dot(glm::normalize(ray.hit - lightPos), glm::normalize(lightDir)) > cos(lightAngle)) {
		color = obj->lighting(lightPos, -ray.dir, ray.hit);
	} else {
		color = ambient * obj->getColor();
	}

	glm::vec3 lightVec = lightPos - ray.hit;
	Ray shadowRay(ray.hit, lightVec);
	shadowRay.closestPt(sceneObjects);

	if(shadowRay.index > -1 && shadowRay.dist < glm::length(lightVec)) {
		color = (ambient + (1 - ambient) * sceneObjects[shadowRay.index]->getTransparencyCoeff()) * obj->getColor();
	}

	if(obj->isTransparent()) {
		float rho = obj->getTransparencyCoeff();
		Ray transmittedRay = Ray(ray.hit, ray.dir);
		glm::vec3 transmittedColor = trace(transmittedRay, step + 1);
		color = (1 - rho) * color + rho * transmittedColor;
	}

	if (obj->isReflective() && step < MAX_STEPS) {
		float rho = obj->getReflectionCoeff();
		glm::vec3 normalVec = obj->normal(ray.hit);
		glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);
		Ray reflectedRay(ray.hit, reflectedDir);
		glm::vec3 reflectedColor = trace(reflectedRay, step + 1);
		color = color + (rho * reflectedColor);
	}

	return color;
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
	float xp, yp;  //grid point
	float cellX = (XMAX-XMIN)/NUMDIV;  //cell width
	float cellY = (YMAX-YMIN)/NUMDIV;  //cell height
	glm::vec3 eye(0., 0., 0.);

	glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glBegin(GL_QUADS);  //Each cell is a tiny quad.

	for(int i = 0; i < NUMDIV; i++)	//Scan every cell of the image plane
	{
		xp = XMIN + i*cellX;
		for(int j = 0; j < NUMDIV; j++)
		{
			yp = YMIN + j*cellY;

			if(AA) {
				glm::vec3 dir1(xp+0.25*cellX, yp+0.25*cellY, -EDIST);
				glm::vec3 dir2(xp+0.75*cellX, yp+0.25*cellY, -EDIST);
				glm::vec3 dir3(xp+0.25*cellX, yp+0.75*cellY, -EDIST);
				glm::vec3 dir4(xp+0.75*cellX, yp+0.75*cellY, -EDIST);

				Ray ray1 = Ray(eye, dir1);
				Ray ray2 = Ray(eye, dir2);
				Ray ray3 = Ray(eye, dir3);
				Ray ray4 = Ray(eye, dir4);

				glm::vec3 col1 = trace(ray1, 1);
				glm::vec3 col2 = trace(ray2, 1);
				glm::vec3 col3 = trace(ray3, 1);
				glm::vec3 col4 = trace(ray4, 1);

				glm::vec3 colAv = (col1 + col2 + col3 + col4) * glm::vec3(0.25);
				glColor3f(colAv.r, colAv.g, colAv.b);
			} else {
				glm::vec3 dir(xp+0.5*cellX, yp+0.5*cellY, -EDIST);	//direction of the primary ray
				Ray ray = Ray(eye, dir);
				glm::vec3 col = trace(ray, 1); //Trace the primary ray and get the colour value
				glColor3f(col.r, col.g, col.b);
			}

			glVertex2f(xp, yp);				//Draw each cell with its color value
			glVertex2f(xp+cellX, yp);
			glVertex2f(xp+cellX, yp+cellY);
			glVertex2f(xp, yp+cellY);
        }
    }

    glEnd();
    glFlush();
}



//---This function initializes the scene ------------------------------------------- 
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{
	texture = TextureBMP("resources/Butterfly.bmp");
	earthTexture = TextureBMP("resources/world_map.bmp");

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);

    glClearColor(0, 0, 0, 1);

    Plane *plane = new Plane (glm::vec3(-40., -15, 0), //Point A
							glm::vec3(40., -15, 0), //Point B
							glm::vec3(40., -15, -100), //Point C
							glm::vec3(-40., -15, -100)); //Point D
	plane->setColor(glm::vec3(1, 1, 0));
	sceneObjects.push_back(plane);

	//earth
	Sphere *sphere0 = new Sphere(glm::vec3(7.0, -5.0, -50.0), 3.0);
	sphere0->setColor(glm::vec3(0, 0, 1));
	sceneObjects.push_back(sphere0);

	Plane *plane2 = new Plane (glm::vec3(12, -10, -50), //Point A
							glm::vec3(22, -10, -50), //Point B
							glm::vec3(22, 0, -50), //Point C
							glm::vec3(12, 0, -50)); //Point D
	plane2->setColor(glm::vec3(1, 1, 0));
	sceneObjects.push_back(plane2);

	Sphere *sphere1 = new Sphere(glm::vec3(-5.0, 0.0, -90.0), 15.0);
	sphere1->setReflectivity(true, 0.8);
	sphere1->setColor(glm::vec3(0, 0, 1));
	sceneObjects.push_back(sphere1);

//	Sphere *sphere2 = new Sphere(glm::vec3(2.0, 2.0, -70.0), 2.0);
//	sphere2->setReflectivity(true, 0.8);
//	sphere2->setColor(glm::vec3(1, 0, 0));   //Set colour to red
//	sceneObjects.push_back(sphere2);		 //Add sphere to scene objects

	Sphere *sphere3 = new Sphere(glm::vec3(-10.0, -5.0, -50.0), 3.0);
	sphere3->setColor(glm::vec3(1., 0., 0.));
	sphere3->setTransparency(true, 0.7);
	sceneObjects.push_back(sphere3);

	Cylinder *cylinder1 = new Cylinder(glm::vec3(1.0, -7.0, -50.0), 2.0, 4.0);
	cylinder1->setColor(glm::vec3(0.5, 0, 0.5));
	sceneObjects.push_back(cylinder1);

	Cone *cone1 = new Cone(glm::vec3(-5.0, -3.0, -50.0), 2.0, 4.0);
	cone1->setColor(glm::vec3(0.5, 0, 0.5));
	sceneObjects.push_back(cone1);

	Cube* cube = new Cube(glm::vec3(-19.0, -5.0, -50.0), 6);
	Plane* planes = cube->planes();
	for(int i = 0; i < 6; i++) {
		(planes[i]).setColor(glm::vec3(1., 0.5, 0.));
		sceneObjects.push_back(&(planes[i]));
	}
}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(650, 650);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracing");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
