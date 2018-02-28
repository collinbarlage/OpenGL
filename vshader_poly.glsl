#version 150

in  vec4 vPosition;
in  vec3 vNormal;

out vec3 fN;
out vec3 fE;
out vec3 fL;

uniform mat4 model_matrix;
uniform mat4 camera_matrix;
uniform mat4 proj_matrix;

uniform vec4 lightPos;

void main() 
{ 
	vec3 pos = (camera_matrix*model_matrix*vPosition).xyz;
	vec3 lightPosInCam = (camera_matrix*lightPos).xyz;

	fN = normalize(camera_matrix*model_matrix*vec4(vNormal, 0)).xyz;
	fE = normalize(vec3(0,0,0)-pos);
	fL = normalize(lightPosInCam.xyz - pos);

	gl_Position = proj_matrix*camera_matrix*model_matrix*vPosition;


}