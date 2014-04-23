layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec4 colour;
layout(location=3) in vec2 texCoord;

uniform mat4 modelMatrix;

smooth out vec3 vNormal;
smooth out vec4 vPosition;
// out vec3 lightDir[4];

void main()
{
	vPosition = transpose(modelMatrix) * vec4(position,1.0);
	
	// for (int i = 0; i < numLights; i++ )
	// {
	// 	lightDir[i] = vec3(lights[0].position - worldPosition.xyz);
	// }

	// passthrough the normal
	vNormal = normal;
	
	gl_Position = projectionMatrix * transpose(viewMatrix) * transpose(modelMatrix) * vec4(position, 1.0);

	
}