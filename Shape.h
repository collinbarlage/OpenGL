#ifndef __Shape_H__
#define __Shape_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <string>
#include <ctime>
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
	vec4 ogColor;
	vector<vec2> points; 
	vector<vec3> verts;
	vector<vec4> colors;
	int numVertices;
	bool multiColor;
	bool brightness;
	vec4 randomColor();
	GLfloat randomFloat();

};
#endif
