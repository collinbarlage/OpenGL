#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

class Sphere: public Drawable{


public:
	Sphere();
	Sphere(unsigned int i);
	~Sphere();

	void init();
	void addVert(vec4 v);
	void addVert(vec4 v, vec4 c);
	void draw(Camera, vector<Light>);
	void makeWireframe();

private:
	void buildSphere();
	void makeTriangle(vec4 a, vec4 b, vec4 c);
	void divideTriangle(vec4 a, vec4 b, vec4 c, int i);
	vec4 norm(vec4 p);
	GLuint vPosition;
	GLuint vColor;
	GLuint mmLoc;
	GLuint cmLoc;
	GLuint pmLoc;
	vec4 randomColor();
	GLfloat randomFloat();

	unsigned int index;
	unsigned int numVerts;
	bool wireframe;

	vector<vec4> points; //6 faces, 2 triangles/face, 3 vertices per triangle
	vector<vec4> colors;
};
#endif
