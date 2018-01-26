#include "Circle.h"
#include <cmath>

Circle::Circle(float radius) {
	srand(time(NULL));
	GLfloat pi =  3.14159265358;

	//get a vertex buffer and a vertex array object
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO); //make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //associate the VBO with the active VAO

	//make circle points with colors
	vector<vec2> points;
	vector<vec4> colors;

	//innitilize circle origin
	NumVertices = 1;
	points.push_back(vec2(0,0));
	colors.push_back(randomColor());
	//draw around the origin according to the radius
	for(GLfloat i=0; i<pi*2; i+= pi/32) {
		points.push_back(vec2(sin(i)*radius, cos(i)*radius));
		colors.push_back(randomColor());
		NumVertices++;
	}
	//complete circle
	points.push_back(points[3]);
	colors.push_back(colors[3]);
	NumVertices++;

	//put the data on the VBO
	glBufferData(GL_ARRAY_BUFFER, 
		points.size() * sizeof(points) + colors.size() * sizeof(colors) , NULL, GL_STATIC_DRAW);
	
	glBufferSubData(GL_ARRAY_BUFFER,0,
		points.size() * sizeof(points) ,&points[0]);

	glBufferSubData(GL_ARRAY_BUFFER,
		points.size() * sizeof(points) ,sizeof(colors) * colors.size(),&colors[0]);

	//Now let's set up the shaders!!
	// Load shaders
	assert((program = InitShader("vshader00_v150.glsl", "fshader00_v150.glsl"))!=-1);
	glUseProgram(program);  //make it the active one

	//get the location of the vPosition and cPosition attribute
	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	assert((cPosition = glGetAttribLocation(program, "cPosition")) != -1);
	glEnableVertexAttribArray(vPosition);  //enable it
	glEnableVertexAttribArray(cPosition);  //enable it
	//associate the area in the active VBO with this attribute and tell it how data to pull out for each vertex from the VBO
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(cPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(points.size() * sizeof(points)));
}

void Circle::draw(Camera cam, vector<Light> lights) {
	//make this object's VAO active
	//this in turn makes it's associate VBO active
	//and the attributes links to the shader
	glBindVertexArray(VAO); 
	glUseProgram(program);  //also switch to using this shader program

	//glUniform4fv(vColor, 1, color);
	
	//now that we're all set up, the the GPU to render NumVertice vertexes as a triangle fan
	//starting at offset 0 in the VBO
	glDrawArrays(GL_TRIANGLE_FAN, 0, NumVertices);
}

vec4 Circle::randomColor() {
	return vec4(randomFloat(), randomFloat(), randomFloat(), 1.0);
}

GLfloat Circle::randomFloat() {
	return ((GLfloat) rand() / (RAND_MAX));
}

