#version 330 core


in vec3 position;
in vec3 vNormal;
in vec2 inUV;

// output values that will be interpolated per-fragment
out vec3 fN;
out vec3 fE;
out vec3 fL1;
out vec3 fL2;
out vec2 uv;


uniform mat4 projectionMatrix;
uniform mat4 viewingMatrix;
uniform mat4 modelMatrix;
uniform vec4 LightPosition1;
uniform vec4 LightPosition2;


void main()
{
	fN = vNormal;
	fE = position.xyz;
	fL1 = (inverse(modelMatrix) * LightPosition1).xyz;
	fL2 = (inverse(modelMatrix) * LightPosition2).xyz;

	//if( LightPosition.w != 0.0 )
	//{
	//	fL = LightPosition.xyz - position.xyz;
	//}

	vec4 transformedPosition = projectionMatrix*viewingMatrix*modelMatrix*vec4(position,1.0f);
    gl_Position = transformedPosition;

    uv = inUV;
}