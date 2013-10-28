#version 330

varying  vec4 ex_Color;
out vec4 out_Color;

uniform float time;
void main(void)
{
	out_Color = vec4(1.0f, time, 0.0f, 1.0f);//ex_Color;
	//out_Color = ex_Color;
}