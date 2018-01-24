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
	Shape(vector<vec2> points, vec4 color);
	void draw(Camera, vector<Light>);
	
private:
	GLuint vPosition;
	GLuint vColor;
	vec4 color;
	int numVertices;
};
#endif
