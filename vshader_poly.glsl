#version 150

in  vec4 vPosition;
in  vec3 vNormal;
in  vec4 vColor;
out vec4 color;
uniform mat4 model_matrix;
uniform mat4 camera_matrix;
uniform mat4 proj_matrix;

uniform vec4 lightPos;
uniform vec4 lightAmbient, lightDiffuse, lightSpecular;
uniform vec4 matAmbient, matDiffuse, matSpecular;
uniform float matAlpha;

void main() 
{ 
	vec3 pos = (camera_matrix*model_matrix*vPosition).xyz;
	vec3 lightPosInCam = (camera_matrix*lightPos).xyz;
	vec3 L = normalize(lightPosInCam.xyz - pos);
	float dist = 1.0;

	vec3 E = normalize(vec3(0,0,0)-pos);
	vec3 N = normalize(camera_matrix*model_matrix*vec4(vNormal, 0)).xyz;
	vec3 H = normalize(L+E);

	vec4 ambient = lightAmbient*matAmbient;

	float Kd = max(dot(L,N),0.0);
	vec4 diffuse = Kd*lightDiffuse*matDiffuse;

	float Ks = pow(max(dot(N,H), 0.0), matAlpha);
	vec4 specular = Ks*lightSpecular*matSpecular;
	if(dot(L,N)<0.0)
		specular = vec4(0,0,0,1);

	color = (ambient+diffuse+specular)*(1/pow(dist,2));
	color.a = 1.0;


	gl_Position = proj_matrix*camera_matrix*model_matrix*vPosition;
}