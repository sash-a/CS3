#version 330 core


// per-fragment interpolated values from the vertex shader
in vec3 fN;
in vec3 fL1;
in vec3 fL2;
in vec3 fE;
in vec2 uv;

out vec4 fColor;

uniform vec4 AmbientProduct, DiffuseProduct1, SpecularProduct1;
uniform vec4 DiffuseProduct2, SpecularProduct2;
uniform mat4 ModelView;
uniform vec4 LightPosition1;
uniform vec4 LightPosition2;
uniform float Shininess;

uniform sampler2D mtexture;

void main()
{
	// Normalize the input lighting vectors
	vec3 N = normalize(fN);
	vec3 E = normalize(fE);
	vec3 L = normalize(fL1);
	vec3 H = normalize(L+E);

	float Kd = max(dot(L, N), 0.0);
	float Ks = pow(max(dot(N, H), 0.0), Shininess);


	vec4 ambient = AmbientProduct;
	//vec4 ambient = vec4(0.1f  ,0.1f,0.1f,1);

	vec4 diffuse = Kd*DiffuseProduct1;
	vec4 specular = Ks*SpecularProduct1;


	// discard the specular highlight if the light’s behind the vertex
	if( dot(L, N) < 0.0 ) 
	{
		specular = vec4(0.0, 0.0, 0.0, 1.0);
	}

	fColor = ambient + diffuse + specular;
	fColor.a = 1.0;

	/////////////////////////////////////////////////////////////////////////

	L = normalize(fL2);
	H = normalize(L+E);

	Kd = max(dot(L, N), 0.0);
	Ks = pow(max(dot(N, H), 0.0), Shininess);


	diffuse = Kd*DiffuseProduct2;
	specular = Ks*SpecularProduct2;


	// discard the specular highlight if the light’s behind the vertex
	if( dot(L, N) < 0.0 ) 
	{
		specular = vec4(0.0, 0.0, 0.0, 1.0);
	}

	fColor += ambient + diffuse + specular;

    //fColor *= texture( mtexture, uv ).rgb;
}