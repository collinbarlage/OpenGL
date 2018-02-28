#include "Polyhedron.h"

Polyhedron::Polyhedron() {
	wireframe = false;
}

Polyhedron::Polyhedron(vec4 d, vec4 s, vec4 a) {
	wireframe = false;
	diff = d;
	spec = s;
	ambi = a;
}

void Polyhedron::init() {
	
	calcNormals();

	//get buffers for attributes and indices
	glGenBuffers(1, &VBO);
	assert((program = InitShader("vshader00_v150.glsl", "fshader00_v150.glsl"))!=-1);
	glUseProgram(program);  //make it the active one
	
	//get vData
	assert((vPosition = glGetAttribLocation(program, "vPosition")) != -1);
	assert((vNormal = glGetAttribLocation(program, "vNormal"))!=-1);

	//get light location
	diffuse_loc = glGetUniformLocation(program, "matDiffuse");
	spec_loc = glGetUniformLocation(program, "matSpecular");
	ambient_loc = glGetUniformLocation(program, "matAmbient");
	alpha_loc = glGetUniformLocation(program, "matAlpha");

	//get the location of the model matrix
	assert((mmLoc = glGetUniformLocation(program, "model_matrix")) != -1);
	assert((cmLoc = glGetUniformLocation(program, "camera_matrix")) != -1);
	assert((pmLoc = glGetUniformLocation(program, "proj_matrix")) != -1);

	//put the data on the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points)*points.size()*2, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points)*points.size(), &points[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points)*points.size(), sizeof(points)*points.size(), &normals[0]);

	//set up stuff for the body of the Polyhedron
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //make this VAO active
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //associate the VBO with the active VAO

	glEnableVertexAttribArray(vPosition);  //enable it
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//get the location of the uniform color in the shader
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)*points.size()));


}

Polyhedron::~Polyhedron(){

}

//----------------------------------------------------------------------------


void Polyhedron::draw(Camera cam, vector<Light> lights){
	//Draw the Polyhedron body
	glBindVertexArray(VAO);
	glUseProgram(program);  //also switch to using this shader program
	glUniformMatrix4fv(mmLoc, 1, GL_TRUE,modelmatrix);
	glUniformMatrix4fv(cmLoc, 1, GL_TRUE,cam.cameraMatrix);
	glUniformMatrix4fv(pmLoc, 1, GL_TRUE,cam.projection);
	glUniform4fv(diffuse_loc, 1, diff);
	glUniform4fv(spec_loc, 1, spec);
	glUniform4fv(ambient_loc, 1, ambi);
	glUniform1f(alpha_loc, 100);
	GLuint light_loc = glGetUniformLocation(program, "lightPos");
	glUniform4fv(light_loc, 1, lights[0].getPosition());
	GLuint ambient_loc2 = glGetUniformLocation(program, "lightAmbient");
	glUniform4fv(ambient_loc2, 1, lights[0].getAmbient());
	GLuint diffuse_loc2 = glGetUniformLocation(program, "lightDiffuse");
	glUniform4fv(diffuse_loc2, 1, lights[0].getDiffuse());
	GLuint specular_loc2 = glGetUniformLocation(program, "lightSpecular");
	glUniform4fv(specular_loc2, 1, lights[0].getSpecular());

	//if(wireframe) {
		//glDrawArrays(GL_TRIANGLES, 0, points.size()/2);
		//glLineWidth(3);
		//for(int i=points.size()/2; i<points.size(); i += 3)
			//glDrawArrays(GL_LINE_LOOP, i, 3);
	//} else {
		glDrawArrays(GL_TRIANGLES, 0, points.size());
	//}
}

void Polyhedron::calcNormals() {
	for(int i=0; i<points.size(); i+=3) {
		vec3 N = normalize(cross(points[i+1]-points[i],points[i+2]-points[i]));
		normals.push_back(N); normals.push_back(N); normals.push_back(N);
	}
}



void Polyhedron::addVert(vec4 v) {
	points.push_back(v);
	//colors.push_back(randomColor());

}

void Polyhedron::addVert(vec4 v, vec4 c) {
	points.push_back(v);
	colors.push_back(c);
}

void Polyhedron::loadSmf(string filename) {
	//load file 
	cout << "Loading file from " << filename << " ...\n";
	ifstream infile(filename.c_str());
	string fin;

	//read
	while (getline(infile, fin)) {
		istringstream iss(fin);
		//split line
		vector<string> tokens;
		copy(istream_iterator<string>(iss),
			istream_iterator<string>(),
			back_inserter(tokens));

		if(!tokens[0].compare("v")){
			vertices.push_back(vec4(stof(tokens[1]), stof(tokens[2]), stof(tokens[3]), 1));
		} else if(!tokens[0].compare("f")) {
			//cout << "ayye" << endl;
			//cout << vertices[stoi(tokens[1])] << endl;
			points.push_back(vertices[stoi(tokens[1])-1]);
			points.push_back(vertices[stoi(tokens[2])-1]);
			points.push_back(vertices[stoi(tokens[3])-1]);
		}
	}
}

vec4 Polyhedron::randomColor() {
	return vec4(randomFloat()+.01, randomFloat()+.1, randomFloat()+.04, 1.0);
}

GLfloat Polyhedron::randomFloat() {
	return ((GLfloat) rand() / (RAND_MAX));
}


void Polyhedron::makeWireframe() {
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

void Polyhedron::pick(vec4 probe, vec4 eye) {

}

void Polyhedron::buildPolyhedron() {
	vec4 c = randomColor();
	points.push_back(vec4(0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-0.5,-1,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-0.5,-1,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,1,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,0.5,-1,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,1,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,1,-0.5,1)); colors.push_back(c);
	c = randomColor();
	
	points.push_back(vec4(0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,1,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(1,0.5,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,-0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(1,-0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,-1,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(1,-0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();

	points.push_back(vec4(0.5,-0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-1,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-1,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,-0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-1,-0.5,1)); colors.push_back(c);
	c = randomColor();
		
	points.push_back(vec4(-0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,1,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(-1,0.5,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,-0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(-1,-0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,-1,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-0.5,-1,1)); colors.push_back(c);
	points.push_back(vec4(-1,-0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();

	//////////////////////////////

	points.push_back(vec4(0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-0.5,1,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-0.5,1,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,0.5,1,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,1,0.5,1)); colors.push_back(c);
	c = randomColor();
	
	points.push_back(vec4(0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,.5,0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(1,0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,-0.5,0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(1,-0.5,0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,-1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(1,-0.5,0.5,1)); colors.push_back(c);
	c = randomColor();

	points.push_back(vec4(0.5,-0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-1,0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,-0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-1,0.5,1)); colors.push_back(c);
	c = randomColor();
		
	points.push_back(vec4(-0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,.5,0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(-1,0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,-0.5,0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(-1,-0.5,0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,-1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-0.5,1,1)); colors.push_back(c);
	points.push_back(vec4(-1,-0.5,0.5,1)); colors.push_back(c);
	c = randomColor();


	/////////////////////

	points.push_back(vec4(-1,0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,-0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,-0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-1,0.5,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,-0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-1,0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,0.5,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,1,-0.5,1)); colors.push_back(c);
	c = randomColor();

	points.push_back(vec4(-1,-0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-1,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-1,-0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-1,-0.5,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-1,-0.5,1)); colors.push_back(c);
	c = randomColor();

	////////////////////////

	points.push_back(vec4(1,0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,-0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,-0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(1,0.5,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,-0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(1,0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,0.5,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,0.5,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,1,-0.5,1)); colors.push_back(c);
	c = randomColor();

	points.push_back(vec4(1,-0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-1,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(1,-0.5,0.5,1)); colors.push_back(c);
	points.push_back(vec4(1,-0.5,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-1,-0.5,1)); colors.push_back(c);
	c = randomColor();

	//////////////////top

	points.push_back(vec4(0.5,-1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-1,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-1,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,-1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,-1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,-1,-0.5,1)); colors.push_back(c);
	c = randomColor();

	points.push_back(vec4(0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,1,-0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,1,-0.5,1)); colors.push_back(c);
	c = randomColor();
	points.push_back(vec4(-0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(0.5,1,0.5,1)); colors.push_back(c);
	points.push_back(vec4(-0.5,1,-0.5,1)); colors.push_back(c);
	c = randomColor();

	
}

