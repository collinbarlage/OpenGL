#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "Angel.h"

class Camera {
public:
	mat4 cameraMatrix;
	void setCameraMatrix(mat4 cm) { cameraMatrix = cm; }
	void move(GLfloat x, GLfloat y, GLfloat z);
};

#endif
