#version 130

//Transformation Vars
uniform vec3 position;
uniform vec3 rotation;
uniform vec3 scale;
uniform vec3 cameraPosition;
uniform mat4 projMatrix;
uniform mat4 viewMatrix;

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
out vec2 textureCoordinates;

void main()
{
	vec4 homogPos = vec4(vPosition, 1.0);
	vec3 angles = radians(rotation);
	vec3 c = cos(angles);
	vec3 s = sin(angles);
	
	//Set Matrices

	//Rotation Matrices
	mat4 rx = mat4(	1.0,	0.0,	0.0,	0.0,
					0.0,	c.x,	s.x,	0.0,
					0.0,	-s.x,	c.x,	0.0,
					0.0,	0.0,	0.0,	1.0);

	mat4 ry = mat4(	c.y,	0.0,	-s.y,	0.0,
					0.0,	1.0,	0.0,	0.0,
					s.y,	0.0,	c.y,	0.0,
					0.0,	0.0,	0.0,	1.0);

	mat4 rz = mat4(	c.z,	-s.z,	0.0,	0.0,
					s.z,	c.z,	0.0,	0.0,
					0.0,	0.0,	1.0,	0.0,
					0.0,	0.0,	0.0,	1.0);

	mat4 rotationMat = rz * ry * rx;

	mat4 translationMat = mat4(	1.0,	0.0,	0.0,	0.0,
								0.0,	1.0,	0.0,	0.0,
								0.0,	0.0,	1.0,	0.0,
								position.x, position.y,	position.z,	1.0);
	
	mat4 scaleMat = mat4(	scale.x,	0.0,	0.0,	0.0,
							0.0,	scale.y,	0.0,	0.0,
							0.0,	0.0,	scale.z,	0.0,
							0.0,	0.0,	0.0,	1.0);

	//Get coordinates relative to the world
	vec4 worldCoord4v = translationMat * rotationMat * scaleMat * homogPos;
	vec4 worldNormal4v = rotationMat * vec4(vNormal, 1.0);

	//Set position relative to camera
	gl_Position = projMatrix * viewMatrix * worldCoord4v;

	//Calculate variables for fragment shader
	vec3 worldCoord3v = vec3(	worldCoord4v.x / worldCoord4v.w,
								worldCoord4v.y / worldCoord4v.w,
								worldCoord4v.z / worldCoord4v.w);

	vec3 worldNormal3v = vec3(	worldNormal4v.x / worldNormal4v.w,
								worldNormal4v.y / worldNormal4v.w,
								worldNormal4v.z / worldNormal4v.w);

	L = normalize(lightPosition - worldCoord3v);
	E = normalize(cameraPosition - worldCoord3v);
	H = normalize(L + E);
	N = normalize(worldNormal3v);

	textureCoordinates = vTexCoord;
}