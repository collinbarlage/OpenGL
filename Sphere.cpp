#include "Sphere.h"

Sphere::Sphere() {
	wireframe = false;
}

Sphere::Sphere(unsigned int i) {
	wireframe = false;
	numVerts = i;
	index = 0;
}

void Sphere::init() {
	
	buildSphere();

	//get buffers for attributes and indices
	glGenBuffers(1, &VBO);

	//put the data on the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points)*points.size() + sizeof(colors)*colors.size(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points)*points.size(), &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points)*points.size(), sizeof(colors)*colors.size(), &colors[0]);

	//set up stuff for the body of the Sphere
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

Sphere::~Sphere(){

}

//----------------------------------------------------------------------------


void Sphere::draw(Camera cam, vector<Light> lights){
	//Draw the Sphere body
	glBindVertexArray(VAO);
	glUseProgram(program);  //also switch to using this shader program
	glUniformMatrix4fv(mmLoc, 1, GL_TRUE,modelmatrix);
	glUniformMatrix4fv(cmLoc, 1, GL_TRUE,cam.cameraMatrix);

	if(wireframe) {
		glDrawArrays(GL_TRIANGLES, 0, points.size()/2);
		glLineWidth(3);
		for(int i=points.size()/2; i<points.size(); i += 3)
			glDrawArrays(GL_LINE_LOOP, i, 3);
	} else {
		glDrawArrays(GL_TRIANGLES, 0, points.size());
	}

}

void Sphere::addVert(vec4 v) {
	points.push_back(v);
	colors.push_back(randomColor());
}

void Sphere::addVert(vec4 v, vec4 c) {
	points.push_back(v);
	colors.push_back(c);
}

vec4 Sphere::randomColor() {
	return vec4(randomFloat()+.01, randomFloat()+.1, randomFloat()+.04, 1.0);
}

GLfloat Sphere::randomFloat() {
	return ((GLfloat) rand() / (RAND_MAX));
}


void Sphere::makeWireframe() {
	wireframe = true;
	//make wireframe
	int s = points.size();
	for(int i=0; i<s; i++) {
		points.push_back(points[i]+vec4(.001*abs(points[i].x)/points[i].x,
			.001*abs(points[i].y)/points[i].y,
			.001*abs(points[i].z)/points[i].z,0));
		colors.push_back(vec4(0,0,0,1));
	}
}

void Sphere::buildSphere() {

	float sqrt2 = (float)sqrt(2);
	float sqrt6 = (float)sqrt(6);
	
	vec4 v[4] = {
		vec4(0,0,1,1),
		vec4(0,2*sqrt2/3,-1.0f/3.0f,1),
		vec4(-sqrt6/3.0f, -sqrt2/3.0f,-1.0f/3.0f,1.0f),
		vec4(sqrt6/3.0f, -sqrt2/3.0f,-1.0f/3.0f,1.0f)
	};

	divideTriangle(v[0], v[1], v[2], 4);
	divideTriangle(v[3], v[2], v[1], 4);
	divideTriangle(v[0], v[3], v[1], 4);
	divideTriangle(v[0], v[2], v[3], 4);
	
}

void Sphere::divideTriangle(vec4 a, vec4 b, vec4 c, int i) {
	cout <<i;
	if(i>0) {
		vec4 v1 = norm(a+b);
		v1.w = 1.0;
		vec4 v2 = norm(a+c);
		v2.w = 1.0;
		vec4 v3 = norm(b+c);
		v3.w = 1.0;

		divideTriangle(a, v1, v2, i-1);
		divideTriangle(c, v2, v3, i-1);
		divideTriangle(b, v3, v1, i-1);
		divideTriangle(v1, v3, v2, i-1);

	} else {
		makeTriangle(a,b,c);
	}
}

void Sphere::makeTriangle(vec4 a, vec4 b, vec4 c) {
	vec4 color = randomColor();
	points.push_back(a);
	points.push_back(b);
	points.push_back(c);
	colors.push_back(color);
	colors.push_back(color);
	colors.push_back(color);
	index +=3;
}


vec4 Sphere::norm(vec4 p) {
	float len = p.x*p.x + p.y*p.y + + p.z*p.z;
	vec4 t;
	if(len > DivideByZeroTolerance) {
		t = p/sqrt(len);
		t.w = 1;
	}
	return t;
}

