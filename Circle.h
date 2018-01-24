#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"
#include "Light.h"
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
	vec4 color;
};
#endif
