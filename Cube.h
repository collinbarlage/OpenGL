#ifndef __CUBE_H__
#define __CUBE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <ctime>
#include <string>
#include <vector>

using namespace std;

class Cube: public Drawable{


public:
	Cube();
	~Cube();

	Cube(vector<vec4> verts);

	void init();
	void addVert(vec4 v);
	void draw(Camera, vector<Light>);

private:
	void buildCube();
	void makeQuad(int, int, int, int);
	GLuint vPosition;
	GLuint vColor;
	GLuint mmLoc;
	GLuint cmLoc;
	GLuint pmLoc;
	vec4 randomColor();
	GLfloat randomFloat();

	unsigned int index;

	vector<vec4> vertices;
	vector<vec4> potentialColors;

	vector<vec4> points; //6 faces, 2 triangles/face, 3 vertices per triangle
	vector<vec4> colors;
};
#endif
