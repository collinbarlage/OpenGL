#include "Angel.h"  //includes gl.h, glut.h and other stuff...
#include "Camera.h"  //for camera objects (for use in future assignments)
#include "Light.h"	//for lights (for use in future assignments)
#include "Polyhedron.h"  //blue box object!
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
Camera cam;
vector<Light> lights;
vector<Drawable*>drawables;

//Helpers
bool animate = false;
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
	mbox = new Polyhedron();

	mbox->init();
	//mbox->setModelMatrix(RotateX(-10)*RotateY(10));

	drawables.push_back(mbox);

}

//----------------------------------------------------------------------------
//Display callback
void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	for (unsigned int i = 0; i < drawables.size(); i++)
		drawables[i]->draw(cam, lights);
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
		cam.rotate(0,0,-1);
		break;
	case 'Z': 
		cam.rotate(0,0,1);
		break;
	case 'c': 
		cam.rotate(0,-1,0);
		break;
	case 'C': 
		cam.rotate(0,1,0);
		break;
	case 'x': 
		cam.rotate(-1,0,0);
		break;
	case 'X': 
		cam.rotate(1,0,0);
		break;
	case ' ': 
		//Rotation animation
		animate = !animate; //toggle
		glutTimerFunc(1, timerCallback, 0);

		break;
	case 'p': case 'P':
		cam.toggleProj();
		break;

	}
	display();
}

void arrows(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		cam.move(-.1,0,0);
		break;

	case GLUT_KEY_RIGHT:
		cam.move(.1,0,0);
		break;

	case GLUT_KEY_UP:
		cam.move(0,0,-.1);
		break;

	case GLUT_KEY_DOWN:
		cam.move(0,0,.1);
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
	if (animate) {
		glutTimerFunc(10, timerCallback, 0);
		glutPostRedisplay();
	}
}

void close(){
	for (unsigned int i = 0; i < drawables.size(); i++)
		delete(drawables[i]);

	if(windowID>0)
			glutDestroyWindow(windowID);

    exit(EXIT_SUCCESS);
}
