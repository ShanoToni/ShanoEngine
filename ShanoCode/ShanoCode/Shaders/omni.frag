#version 410

in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
    float lightDistance = length(FragPos.xyz - lightPos);

    //map to [0, 1]
    lightDistance = lightDistance / far_plane;

    gl_FragDepth = lightDistance;
}