in vec4 ex_Colour;

out vec4 gl_FragColor;

void main()
{
	// gl_FragColor = vec4(1.0,0,0,0.2);
	// ex_Colour.a = 0.2;
	gl_FragColor = ex_Colour;
}