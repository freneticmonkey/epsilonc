in vec3 vNormal;
in vec4 vPosition;
in vec4 vShadowCoord;

in vec2 vTexCoord;

uniform sampler2D theTexture;

out vec4 fragColor;

vec4 spotLight(int lightID)
{
	float nDotVP;       // normal * light direction
    float nDotR;        // normal * light reflection vector
    float pf;           // power factor
    float spotDot;      // cosine of angle between spotlight
    float spot_att;     // spotlight attenuation factor;
    float attenuation;  // computed attenuation factor
    float d;            // distance from surface to light position
    vec3 VP;            // direction from surface to light position
    vec3 reflection;    // direction of maximum highlights
 
    // Compute vector from surface to light position
    VP = vec3 (lights[lightID].position) - vPosition.xyz;
 
    // Compute distance between surface and light position
    d = length (VP);
 
    // Normalize the vector from surface to light position
    VP = normalize (VP);
 
    // Compute attenuation
    attenuation = 1.f / (lights[lightID].attenuation[0] +
                         lights[lightID].attenuation[1] * d +
                         lights[lightID].attenuation[2] * d * d);

 
    // See if point on surface is inside cone of illumination
    spotDot = dot (-VP, normalize (lights[lightID].direction));
 
    if (spotDot < lights[lightID].spotCutoff)
        spot_att = 0.f;
    else
        spot_att = pow (spotDot, lights[lightID].spotExponent);
 
    // Combine the spot and distance attenuation
    attenuation *= spot_att;
 
    reflection = normalize (reflect (-normalize (VP), normalize
                (vNormal)));
 
    nDotVP = max (0.f, dot (vNormal, VP));
    nDotR = max (0.f, dot (normalize (vNormal), reflection));
 
    if (nDotVP == 0.f)
        pf = 0.f;
    else
        pf = pow (nDotR, material.reflectance);

    // Shadowing
    float visibility = 1.0;
    if ( texture(shadowMap, vShadowCoord.xy).z < vShadowCoord.z)
    {
        visibility = 0.5;
    }
 
    vec4 ambient    = material.ambient  * lights[lightID].ambient  * attenuation;
    vec4 diffuse    = material.diffuse  * lights[lightID].diffuse  * nDotVP * attenuation;
    vec4 specular   = material.specular * lights[lightID].specular * pf * attenuation;
    
    vec4 lightColour = ambient + (visibility * diffuse) + ( visibility * specular) + (texture(theTexture, vTexCoord) * 0.5);
    
    return lightColour;
}

vec4 pointLight (int lightID)
{
    float nDotVP;       // normal * light direction
    float nDotR;        // normal * light reflection vector
    float pf;           // power factor
    float attenuation;  // computed attenuation factor
    float d;            // distance from surface to light position
    vec3 VP;            // direction from surface to light position
    vec3 reflection;    // direction of maximum highlights
 
    // Compute vector from surface to light position
    VP = vec3 (lights[lightID].position) - vPosition.xyz;
 
    // Compute distance between surface and light position
    d = length (VP);
 
    // Normalize the vector from surface to light position
    VP = normalize (VP);
 
    // Compute attenuation
    attenuation = 1.f / (lights[lightID].attenuation[0] +
                         lights[lightID].attenuation[1] * d +
                         lights[lightID].attenuation[2] * d * d);
 
    reflection = normalize (reflect (-normalize (VP), normalize
                (vNormal)));
 
    nDotVP = max (0.f, dot (vNormal, VP));
    nDotR = max (0.f, dot (normalize (vNormal), reflection));
 
    if (nDotVP == 0.f)
        pf = 0.f;
    else
        pf = pow (nDotR, 30);
        // pf = pow (nDotR, material.reflectance);
 
    vec4 ambient = material.ambient * lights[lightID].ambient * attenuation;
    vec4 diffuse = material.diffuse * lights[lightID].diffuse * nDotVP * attenuation;
    vec4 specular = material.specular * lights[lightID].specular * pf * attenuation;
 
    return ambient + diffuse + specular;
    // return diffuse;
}


void main()
{
	vec4 final_color = vec4(0.2, 0.2, 0.2, 1.0);
	
	for (int i = 0; i < numLights; i++ )
	{
        if ( lights[i].type == 0)
        {
            final_color += pointLight(i);
        }
        else if ( lights[i].type == 1)
        {
             final_color += spotLight(i);
        }

		// final_color += spotLight(i);
		// vec3 lightDir = vec3(lights[0].position - worldPosition.xyz);
		
		// vec3 N = normalize(vNormal);
		// vec3 L = normalize(lightDir);
		// float lambertTerm = dot(N,L);
		// lambertTerm = clamp(lambertTerm, 0.0, 1.0);

		// if (lambertTerm > 0.0)
	 //    {
	 //        final_color += lights[i].diffuse * material.diffuse * lambertTerm * 1.0;
	 //    }
	}

    fragColor = final_color;
    // fragColor = material.diffuse;
}