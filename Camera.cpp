#include "Camera.h"

void Camera::move(GLfloat x, GLfloat y, GLfloat z) {
	cameraMatrix *= Translate(x,y,z);
}
//TODO