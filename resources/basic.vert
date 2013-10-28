#version 330

layout(location=1) in vec4 position;
layout(location=2) in vec3 normal;
layout(location=3) in vec4 colour;

uniform mat4 modelViewMatrix;
uniform mat4 projMatrix;

varying vec4 ex_Colour;

void main(void)
{
	//vec4 pos = in_Position + vec4(0, 0, -1, 0);//object_Position;
	vec4 pos = modelViewMatrix * position;// + object_Position;
	//pos = projMat * viewMat * pos;
	gl_Position = projMatrix * pos;
	
	ex_Colour = colour;//Vec4(0.0, 0.0, 0.0, 1.0);
}