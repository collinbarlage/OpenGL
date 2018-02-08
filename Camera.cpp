#include "Camera.h"


Camera::Camera() {

	projection = Perspective(65,1,1,100);
	ortho = false;

	xA = 0;
	yA = 1.5707963267948966;
	zA = 0;

	eye = vec4(2,2,2,1);
	at  = vec4(0,0,0,1);
	up  = vec4(0,1,0,1);

	setCameraMatrix(projection*LookAt(eye, at, up));
}

void Camera::move(GLfloat x, GLfloat y, GLfloat z) {
	at += vec4(x,y,z,0);
	eye += vec4(x,y,z,0);

	setCameraMatrix(cameraMatrix*Translate(x,y,z));
}

void Camera::rotate(GLfloat x, GLfloat y, GLfloat z) {
	xA += x; yA += y; zA += z;
	setCameraMatrix(cameraMatrix*RotateX(x)*RotateY(y)*RotateZ(z));
}

void Camera::toggleProj() {
	if(ortho){
		projection = Perspective(65,1,1,100);
		ortho = false;
	} else {
		projection = Ortho(-1,1,-1,1,-10,10);
		ortho = true;
	}

	calcProj();
}

void Camera::calcProj() {
	setCameraMatrix(projection*LookAt(eye, at, up));
	setCameraMatrix(cameraMatrix*RotateX(xA)*RotateY(yA)*RotateZ(zA));

}