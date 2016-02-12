#version 130

//Transformation Vars
uniform mat4 transformMatrix;
uniform mat3 rotationMatrix;
uniform vec3 cameraPosition;
uniform mat4 projMatrix;
uniform mat4 viewMatrix;

//Color vars
uniform vec4 color;

//Shading Vars
uniform vec3 lightPosition;
uniform sampler2D tex;

//Attribute Vars
in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

//Fragment Shader Vars
out vec3 L;
out vec3 E;
out vec3 H;
out vec3 N;

void main()
{
	vec4 homogPos = vec4(vPosition, 1.0);

	//Get coordinates relative to the world
	vec4 worldCoord4v = transformMatrix * homogPos;
	vec3 worldNormal3v = rotationMatrix * vNormal;

	//Set position relative to camera
	gl_Position = projMatrix * viewMatrix * worldCoord4v;

	//Calculate variables for fragment shader
	vec3 worldCoord3v = vec3(	worldCoord4v.x / worldCoord4v.w,
								worldCoord4v.y / worldCoord4v.w,
								worldCoord4v.z / worldCoord4v.w);

	L = normalize(lightPosition - worldCoord3v);
	E = normalize(cameraPosition - worldCoord3v);
	H = normalize(L + E);
	N = normalize(worldNormal3v);
}