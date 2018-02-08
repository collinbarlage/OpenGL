#include "Cube.h"

Cube::Cube() {

}

void Cube::init() {

	//make room for colors and points
	for(int i=0; i<vertices.size()*2*6; i++) {
		points.push_back(vec4(0,0,0,1));
		colors.push_back(vec4(0,1,1,1));
	}

	buildCube();

	//get buffers for attributes and indices
	glGenBuffers(1, &VBO);

	//put the data on the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points)*points.size() + sizeof(colors)*colors.size(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points)*points.size(), &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points)*points.size(), sizeof(colors)*colors.size(), &colors[0]);

	//set up stuff for the body of the cube
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //associate the VBO with the active VAO

	assert((program = InitShader("vshader00_v150.glsl", "fshader00_v150.glsl"))!=-1);
	glUseProgram(program);  //make it the active one

	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	glEnableVertexAttribArray(vPosition);  //enable it
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//get the location of the uniform color in the shader
	assert((vColor = glGetAttribLocation(program, "vColor"))!=-1);
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)*points.size()));

	//get the location of the model matrix
	assert((mmLoc = glGetUniformLocation(program, "model_matrix")) != -1);
	assert((cmLoc = glGetUniformLocation(program, "camera_matrix")) != -1);
	

}

Cube::~Cube(){

}

//----------------------------------------------------------------------------
void Cube::makeQuad(int ind1, int ind2, int ind3, int ind4) {
	//triangle #1
	points[index] = vertices[ind1];
	colors[index] = potentialColors[index%8];
	index++;

	points[index] = vertices[ind2];
	colors[index] = colors[index - 1];
	index++;

	points[index] = vertices[ind3]; 
	colors[index] = colors[index - 1];
	index++;

	//Triangle #2

	points[index] = vertices[ind3];
	colors[index] = colors[index - 1];
	index++;

	points[index] = vertices[ind4];
	colors[index] = colors[index - 1];
	index++;

	points[index] = vertices[ind1];
	colors[index] = colors[index - 1];
	index++;
}

void Cube::buildCube() {
	index = 0;

	makeQuad(1, 0, 3, 2);  //front
	makeQuad(2, 3, 7, 6);  //right
	makeQuad(3, 0, 4, 7);  //bottom
	makeQuad(6, 5, 1, 2);  //top
	makeQuad(4, 5, 6, 7);  //back
	makeQuad(5, 4, 0, 1);  //left
}

void Cube::draw(Camera cam, vector<Light> lights){
	//Draw the cube body
	glBindVertexArray(VAO);
	glUseProgram(program);  //also switch to using this shader program
	glUniformMatrix4fv(mmLoc, 1, GL_TRUE,modelmatrix);
	glUniformMatrix4fv(cmLoc, 1, GL_TRUE,cam.cameraMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 2 * 3);

}

void Cube::addVert(vec4 v) {
	vertices.push_back(v);
	potentialColors.push_back(randomColor());
}

vec4 Cube::randomColor() {
	return vec4(randomFloat(), randomFloat(), randomFloat(), 1.0);
}

GLfloat Cube::randomFloat() {
	return ((GLfloat) rand() / (RAND_MAX));
}
