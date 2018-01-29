#ifndef __Shape_H__
#define __Shape_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <vector>

using namespace std;

class Shape: public Drawable{


public:	
	Shape();
	Shape(vec4 color);
	void init();
	void draw(Camera, vector<Light>);
	void trans(mat3 m);
	void addVerts(GLfloat x, GLfloat y);
	
private:
	GLuint vPosition;
	GLuint vColor;
	GLuint cPosition;
	vec4 color;
	vector<vec3> verts;
	int numVertices;
	bool multiColor;
	vec4 randomColor();
	GLfloat randomFloat();

};
#endif
