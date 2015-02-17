#version 130

//In Vars
in vec3 L;
in vec3 E;
in vec3 H;
in vec3 N;
in vec2 textureCoordinates;

//Uniforms
uniform sampler2D tex;

void main()
{
	vec3 ambient = vec3(.4);
	vec3 diffuse = vec3(.4);
	vec3 specular = vec3(.2);

	float gloss = .7;

	//Get color of pixel from texture
	vec4 hue = texture(tex, textureCoordinates);

	//Light Science
	diffuse *= max(dot(L,N), 0.0);
	specular *= pow(max(dot(N,H), 0.0), gloss);
	if(dot(L, N) < 0.0) specular = vec3(0.0);

	gl_FragColor = vec4(((ambient + diffuse) * hue.rgb + specular), hue.a);
}