// Ouput data
layout(location = 0) out float fragmentdepth;
 
void main()
{
    // Not really needed, OpenGL does it anyway
    fragmentdepth = fract(sin(dot(gl_FragCoord.xy ,vec2(12.9898,78.233))) * 43758.5453);//gl_FragCoord.z;
}