#include "Shape.h"

Shape::Shape() {
	multiColor = true;
	brightness = true;
	numVertices = 0;
}

Shape::Shape(vec4 c) {
	multiColor = false;
	brightness = true;
	color = c;// RGBA colors
	ogColor = color;
	numVertices = 0;
}

void Shape::init() {
	srand(time(NULL));

	//get a vertex buffer and a vertex array object
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO); //make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //associate the VBO with the active VAO

	numVertices = verts.size();


	vector<vec2> points; //strip homogenius coords for GL
	for(int i=0; i< numVertices; i++) {
		points.push_back(vec2(verts[i].x, verts[i].y));
		if(colors.size() < points.size()) {
			colors.push_back(randomColor());
		}
	}

	if(multiColor) {
		//put the data on the VBO
		glBufferData(GL_ARRAY_BUFFER,points.size() * sizeof(points) + colors.size() * sizeof(colors) , NULL, GL_STATIC_DRAW);
	
		glBufferSubData(GL_ARRAY_BUFFER,0,points.size() * sizeof(points) ,&points[0]);

		glBufferSubData(GL_ARRAY_BUFFER,points.size() * sizeof(points) ,sizeof(colors) * colors.size(),&colors[0]);

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

	} else {
		//put the data on the VBO
		glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(points), &points[0], GL_STATIC_DRAW);
	
		//Now let's set up the shaders!!
		// Load shaders
		assert((program = InitShader("vshader00_uniform.glsl", "fshader00_uniform.glsl"))!=-1);
		glUseProgram(program);  //make it the active one

		//get the location of the vPosition attribute
		assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
		glEnableVertexAttribArray(vPosition);  //enable it
		//associate the area in the active VBO with this attribute and tell it how data to pull out for each vertex from the VBO
		glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

		//get the location of the uniform color in the shader
		assert((vColor = glGetUniformLocation(program, "color"))!=-1);
	}
}

void Shape::draw(Camera cam, vector<Light> lights){
	//make this object's VAO active
	//this in turn makes it's associate VBO active
	//and the attributes links to the shader
	glBindVertexArray(VAO); 
	glUseProgram(program);  //also switch to using this shader program

	//unfortunately, every time we draw we still need to set again all uniform variables
	glUniform4fv(vColor, 1, color);

	//now that we're all set up, the the GPU to render NumVertice vertexes as a triangle fan
	//starting at offset 0 in the VBO
	glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices);

}

void Shape::addVerts(GLfloat x, GLfloat y) {
	verts.push_back(vec3(x,y,1)); //make homogenius point
	numVertices++;
}


void Shape::trans(mat3 m) {
	vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
	//move verts
	for(int i=0; i<numVertices; i++) {
		verts[i] = m*verts[i];
	}

	//change brightnes
	//cout << color << endl;
	if(brightness) {
		if(color.x < black.x && color.y < black.y && color.z < black.z) {
			brightness = false;
		} else {
			color += vec4(-0.007, -0.007, -0.007, 0);
		}
	} else {
		if(color.x > ogColor.x && color.y > ogColor.y && color.z > ogColor.z) {
			brightness = true;
		} else {
			color += vec4(0.007, 0.007, 0.007, 0);
		}
	}
}

vec4 Shape::randomColor() {
	return vec4(randomFloat(), randomFloat(), randomFloat(), 1.0);
}

GLfloat Shape::randomFloat() {
	return ((GLfloat) rand() / (RAND_MAX));
}


