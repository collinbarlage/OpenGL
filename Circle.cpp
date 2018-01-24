#include "Circle.h"
#include <cmath>

Circle::Circle(float radius) {
	GLfloat pi =  3.14159265358;

	//get a vertex buffer and a vertex array object
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO); //make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //associate the VBO with the active VAO

	color = vec4(0.0, 1.0, 0.0, 1.0);

	//make circle points with colors
	vector<vec2> points;
	//vector<vec4> colors;
	//innitilize circle points
	NumVertices = 1;
	points.push_back(vec2(0,0));
	//colors.push_back(vec4(0.0, 0.0, 0.0, 1.0));

	for(GLfloat i=0; i<pi*2; i+= pi/24) {
		points.push_back(vec2(sin(i)*radius, cos(i)*radius));
		//colors.push_back(vec4(0.5, 0.9, 1.0, 1.0));
		NumVertices++;
	}
	//complete circle
	points.push_back(vec2(0, 1*radius));
	//colors.push_back(vec4(0.5, 0.9, 1.0, 1.0));

	NumVertices++;

	//put the data on the VBO
	glBufferData(GL_ARRAY_BUFFER, 
		points.size() * sizeof(points) , &points[0], GL_STATIC_DRAW);
	//+ colors.size() * sizeof(colors)
	
	//Now let's set up the shaders!!
	// Load shaders
	assert((program = InitShader("vshader00_v150.glsl", "fshader00_v150.glsl"))!=-1);
	glUseProgram(program);  //make it the active one

	//get the location of the vPosition attribute
	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	glEnableVertexAttribArray(vPosition);  //enable it
	//associate the area in the active VBO with this attribute and tell it how data to pull out for each vertex from the VBO
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//set up color attribute?
	//GLuint vPosition = glGetAttribLocation(program, "vPosition");
	//glEnableVertexAttribArray(vPosition);
	
	//GLuint vColor = glGetAttribLocation(program, "vColor");
	//glEnableVertexAttribArray(vColor);
	//glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(NumVertices));

	//get the location of the uniform color in the shader
	assert((vColor = glGetUniformLocation(program, "color"))!=-1);

}

void Circle::draw(Camera cam, vector<Light> lights){
	//make this object's VAO active
	//this in turn makes it's associate VBO active
	//and the attributes links to the shader
	glBindVertexArray(VAO); 
	glUseProgram(program);  //also switch to using this shader program

	glUniform4fv(vColor, 1, color);
	//glFragmentColorMaterialEXT(vColor, 1);

	//now that we're all set up, the the GPU to render NumVertice vertexes as a triangle fan
	//starting at offset 0 in the VBO
	glDrawArrays(GL_TRIANGLE_FAN, 0, NumVertices);

}
