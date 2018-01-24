#include "Angel.h"  //includes gl.h, glut.h and other stuff...
#include "Camera.h"  //for camera objects (for use in future assignments)
#include "Light.h"	//for lights (for use in future assignments)
#include "Shape.h"  //blue box object!
#include "Circle.h"  

//Forward declarations
void init(void);
void display(void);
void keyboard(unsigned char, int, int);
void resize(int width, int height);
void close(void);

//Objects
Shape* mbox;
Circle* mcircle;
Camera cam;
vector<Light> lights;
vector<Drawable*>drawables;
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

	//Red square
	vector<vec2> square;
	square.push_back(vec2(0.25, 0.25));
	square.push_back(vec2(0.75, 0.25));
	square.push_back(vec2(0.75, 0.75));
	square.push_back(vec2(0.25, 0.75));
	mbox = new Shape(square, vec4(1.0, 0.0, 0.0, 1.0));
	drawables.push_back(mbox);

	//Blue triangle
	vector<vec2> triangle;
	triangle.push_back(vec2(-1, -1));
	triangle.push_back(vec2(0, -1));
	triangle.push_back(vec2(0, 0));
	mbox = new Shape(triangle, vec4(0.0, 0.0, 1.0, 1.0));
	drawables.push_back(mbox);

	//Multicolored circle
	mcircle = new Circle(.2);
	drawables.push_back(mcircle);

	

}


//----------------------------------------------------------------------------
//Display callback
void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );

	for (unsigned int i = 0; i < drawables.size(); i++)
		drawables[i]->draw(cam, lights);
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