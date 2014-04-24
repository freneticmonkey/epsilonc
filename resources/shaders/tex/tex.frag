in vec4 ex_Colour;
in vec2 vTexCoord;
uniform sampler2D theTexture;

out vec4 fragColor;

void main()
{
	// fragColor = ex_Colour;
	fragColor = texture(theTexture, vTexCoord);
}