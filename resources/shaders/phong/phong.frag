in vec4 ex_Colour;
in vec3 norm;
in vec3 lightDir;

out vec4 fragColor;

void main()
{
	// fragColor = ex_Colour;
	fragColor = vec4(norm.x, norm.y, norm.z, 1.0);

	vec4 final_color = vec4(0.2, 0.2, 0.2, 1.0);
	vec3 N = normalize(norm);
	vec3 L = normalize(lightDir);
	float lambertTerm = dot(N,L);
	lambertTerm = clamp(lambertTerm, 0.0, 1.0);

	if (lambertTerm > 0.0)
    {
        final_color += vec4(1.0, 1.0, 1.0, 1.0) * ex_Colour * lambertTerm;
    }
    fragColor = final_color;
}