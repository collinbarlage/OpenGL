#include "Angel.h"  //includes gl.h, glut.h and other stuff...
#include "Camera.h"  //for camera objects (for use in future assignments)
#include "Light.h"	//for lights (for use in future assignments)
#include "Polyhedron.h"  //blue box object!
#include "Sphere.h"  //a ball!?
#include <cstdlib>
#include <ctime>

//Forward declarations
void init(void);
void display(void);
void keyboard(unsigned char, int, int);
void arrows(int key, int x, int y);
void resize(int width, int height);
void close(void);
void timerCallback(int value);


//Objects
Polyhedron* mbox;
Sphere* sphere;
Camera cam1 = Camera(vec4(0,0,2,1), vec4(0,1,0,1));
Camera cam2 = Camera(vec4(0,10,0,1), vec4(0,0,-1,1));
vector<Light> lights;
vector<Drawable*>drawables;

//Helpers
bool camSelect = false;
GLuint windowID=0;


//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
	srand(time(NULL));
	//initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);

	windowID = glutCreateWindow("CS 432 - Collin Barlage - OpenGL");

	//print out info about our system
	fprintf(stdout, "OpenGL Version: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//initialize glew 
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	init(); //initalize objects

	//set up the callback functions
	glutDisplayFunc(display);
	glutWMCloseFunc(close);
	glutKeyboardFunc(keyboard);  //What to do if a keyboard event is detected
	glutSpecialFunc(arrows);


	//start the main event listening loop
	glutMainLoop();
	return 0;
}

// Initialization
void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	//scene

	//floor plane
	mbox = new Polyhedron();
	mbox->addVert(vec4(1,0,1,1), vec4(.7,.8,.8,1));
	mbox->addVert(vec4(1,0,-1,1), vec4(.7,.8,.8,1));
	mbox->addVert(vec4(-1,0,-1,1), vec4(.7,.8,.8,1));

	mbox->addVert(vec4(-1,0,1,1), vec4(.7,.8,.8,1));
	mbox->addVert(vec4(1,0,1,1), vec4(.7,.8,.8,1));
	mbox->addVert(vec4(-1,0,-1,1), vec4(.7,.8,.8,1));
	mbox->init();
	//mbox->setModelMatrix(Translate(0,1,-4));
	drawables.push_back(mbox);


	//sphere
	sphere = new Sphere(64);
	sphere->init();
	drawables.push_back(sphere);



}

//----------------------------------------------------------------------------
//Display callback
void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//use correct camera
	Camera camera;
	if(camSelect) {
		camera = cam2;
	} else {
		camera = cam1;
	}

	for (unsigned int i = 0; i < drawables.size(); i++)
		drawables[i]->draw(camera, lights);
	glutSwapBuffers();
}

//----------------------------------------------------------------------------
//Window resize callback
void resize(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);  //make the viewport the entire window
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033:  // Escape key
	case 'q': case 'Q':
		close();
		break;
	case 'z': 
		cam1.rotate(0,0,-1);
		break;
	case 'Z': 
		cam1.rotate(0,0,1);
		break;
	case 'c': 
		cam1.rotate(0,-1,0);
		break;
	case 'C': 
		cam1.rotate(0,1,0);
		break;
	case 'x': 
		cam1.rotate(-1,0,0);
		break;
	case 'X': 
		cam1.rotate(1,0,0);
		break;
	case ' ': 
		//Camera toggle
		camSelect = !camSelect; //toggle
		//glutTimerFunc(1, timerCallback, 0);

		break;
	case 'p': case 'P':
		cam1.toggleProj();
		break;

	}
	display();
}

void arrows(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		cam1.move(-.1,0,0);
		break;

	case GLUT_KEY_RIGHT:
		cam1.move(.1,0,0);
		break;

	case GLUT_KEY_UP:
		cam1.move(0,0,-.1);
		break;

	case GLUT_KEY_DOWN:
		cam1.move(0,0,.1);
		break;
	}

	display();
}

//----------------------------------------------------------------------------
//Timer  callback
void timerCallback(int value) {
	vec2 pos;
	//Rotate all objects
	for(int i=0; i<drawables.size(); i++) {
		//pos = drawables[i]->position;
		//rotate
		drawables[i]->setModelMatrix(RotateY(1));
	}
	//continue rotating unless spacebar was pressed to toggle animation
	//if (animate) {
		//glutTimerFunc(10, timerCallback, 0);
		//glutPostRedisplay();
	//}
}

void close(){
	for (unsigned int i = 0; i < drawables.size(); i++)
		delete(drawables[i]);

	if(windowID>0)
			glutDestroyWindow(windowID);

    exit(EXIT_SUCCESS);
}
