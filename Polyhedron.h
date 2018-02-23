#ifndef __Polyhedron_H__
#define __Polyhedron_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

class Polyhedron: public Drawable{


public:
	Polyhedron();
	~Polyhedron();

	Polyhedron(vector<vec4> verts);

	void init();
	void addVert(vec4 v);
	void addVert(vec4 v, vec4 c);
	void draw(Camera, vector<Light>);
	void pick(vec4 probe, vec4 eye);
	void makeWireframe();

private:
	void buildPolyhedron();
	void makeQuad(int, int, int, int);
	GLuint vPosition;
	GLuint vColor;
	GLuint mmLoc;
	GLuint cmLoc;
	GLuint pmLoc;
	vec4 randomColor();
	GLfloat randomFloat();

	unsigned int index;
	bool wireframe;

	vector<vec4> vertices;
	vector<vec4> potentialColors;

	vector<vec4> points; //6 faces, 2 triangles/face, 3 vertices per triangle
	vector<vec4> colors;
};
#endif
