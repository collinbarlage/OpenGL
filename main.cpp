#include "Angel.h"  //includes gl.h, glut.h and other stuff...
#include "Camera.h"  //for camera objects (for use in future assignments)
#include "Light.h"	//for lights (for use in future assignments)
#include "Shape.h"  //blue box object!
#include "Circle.h"  

//Forward declarations
void init(void);
void display(void);
void keyboard(unsigned char, int, int);
void mouse(GLint button, GLint state, GLint x, GLint y);
void resize(int width, int height);
void close(void);
void timerCallback(int value);

//Transformations
mat3 transl(vec2 t);
mat3 rotate(float rad);
mat3 scale(float s);

//Objects
Shape* mbox;
Circle* mcircle;
Camera cam;
vector<Light> lights;
vector<Drawable*>drawables;

//Helpers
bool animate = false; 
//----------------------------------------------------------------------------


int main(int argc, char **argv)
{
	//initialize GULT
	glutInit(&argc, argv);  
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutInitWindowSize(512, 512);

	glutCreateWindow("CS 432 - OpenGL - COLLIN BARLAGE");
	//print out info about our system
	fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	//initialize glew on non-apple systems

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	
	init(); //initalize objects

	//set up the callback functions
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutWMCloseFunc(close);

	//start the main event listening loop
	glutMainLoop();
	return 0;
}

// Initialization
void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	
	//Scene

	


	//Blue triangle
	/*
	vector<vec2> triangle;
	triangle.push_back(vec2(-1, -1));
	triangle.push_back(vec2(0, -1));
	triangle.push_back(vec2(0, 0));
	mbox = new Shape(triangle, vec4(0.0, 0.0, 1.0, 1.0));
	drawables.push_back(mbox);

	//Multicolored circle
	mcircle = new Circle(.2);
	drawables.push_back(mcircle);
	*/

	

}


//----------------------------------------------------------------------------
//Display callback
void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );

	for (unsigned int i = 0; i < drawables.size(); i++) {
		drawables[i]->draw(cam, lights);
	}
	glFlush();
}

//----------------------------------------------------------------------------
//Keyboard even callback
void keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
	case 033:  // Escape key
	case 'q': case 'Q':
		close();
		exit(EXIT_SUCCESS);
	    break;
	case ' ': 
		//Rotation animation
		animate = !animate; //toggle
		glutTimerFunc(1, timerCallback, 0);

		break;
    }
}

//----------------------------------------------------------------------------
//Window resize callback
void resize(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);  //make the viewport the entire window
}

void close(){
	for (unsigned int i = 0; i < drawables.size(); i++)
		delete(drawables[i]);

}

//----------------------------------------------------------------------------
//Mouse resize callback
void mouse(GLint button, GLint state, GLint x, GLint y) {
	GLfloat dx = (GLfloat(x)-250)/250;
	GLfloat dy = ((GLfloat(y)-250)*-1)/250;
	

	//Red square
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		mbox = new Shape(vec4(1.0, 0.0, 0.0, 1.0));
		if(glutGetModifiers()  && GLUT_ACTIVE_SHIFT)  //check for multicolor
			mbox = new Shape();
		mbox->addVerts(0.15, 0.15);
		mbox->addVerts(-0.15, 0.15);
		mbox->addVerts(-0.15, -0.15);
		mbox->addVerts(0.15, -0.15);
		mbox->trans(transl(vec2(dx,dy)));
		mbox->position = vec2(dx, dy);
		mbox->init();
		drawables.push_back(mbox);
	} 

	//Blue triangle
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		mbox = new Shape(vec4(0.0, 0.0, 1.0, 1.0));
		if(glutGetModifiers()  && GLUT_ACTIVE_SHIFT)  //check for multicolor
			mbox = new Shape();
		mbox->addVerts(-.13, -.13);
		mbox->addVerts(-.13, .17);
		mbox->addVerts(.17, -.13);
		mbox->position = vec2(dx, dy);
		mbox->trans(transl(vec2(dx,dy)));
		mbox->init();
		drawables.push_back(mbox);
	} 


}
//----------------------------------------------------------------------------
//Timer  callback
void timerCallback(int value) {
	vec2 pos;
	//Rotate all objects
	for(int i=0; i<drawables.size(); i++) {
		pos = drawables[i]->position;
		drawables[i]->trans(transl(pos)*rotate(.03)*transl(vec2(-pos.x, -pos.y)));
		//drawables[i]->setModelMatrix(transl(pos)*rotate(.02)*transl(vec2(-pos.x, -pos.y)));
		//drawables[i]->init();
	}

	//continue rotating unless spacebar was pressed to toggle animation
	if (animate) {
		glutTimerFunc(5, timerCallback, 0);
		glutPostRedisplay();
	}
}

//Helper functions


//Transformations
mat3 transl(vec2 t) {
	return mat3(vec3(1, 0, t.x),
				vec3(0, 1, t.y),
				vec3(0, 0, 1));
}

mat3 rotate(float rad) {
	return mat3(vec3(cos(rad), -sin(rad), 0),
				vec3(sin(rad),  cos(rad), 0),
				vec3(0       ,  0       , 1));
}

mat3 scale(float s) {
	return mat3(vec3(s, 0, 0),
				vec3(0, s, 0),
				vec3(0, 0, 1));
}