#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

using namespace std;

class Circle: public Drawable{


public:	
	Circle();
	Circle(float radius);
	void draw(Camera, vector<Light>);
	
private:
	int NumVertices;
	GLuint vPosition;
	GLuint vColor;
	GLuint cPosition;
	vec4 color;
	vec4 randomColor();
	GLfloat randomFloat();
};
#endif
