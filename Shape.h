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
	void addPoint(vec2 p);
	
private:
	GLuint vPosition;
	GLuint vColor;
	vec4 color;
	vector<vec2> points;
	int numVertices;
};
#endif
