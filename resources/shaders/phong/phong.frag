in vec3 vNormal;
in vec4 worldPosition;

out vec4 fragColor;

void main()
{
	vec4 final_color = vec4(0.2, 0.2, 0.2, 1.0);
	
	for (int i = 0; i < numLights; i++ )
	{
		vec3 lightDir = vec3(lights[0].position - worldPosition.xyz);
		
		vec3 N = normalize(vNormal);
		vec3 L = normalize(lightDir);
		float lambertTerm = dot(N,L);
		lambertTerm = clamp(lambertTerm, 0.0, 1.0);

		if (lambertTerm > 0.0)
	    {
	        final_color += lights[i].diffuse * material.diffuse * lambertTerm * 1.0;
	    }
	}

    fragColor = final_color;
}