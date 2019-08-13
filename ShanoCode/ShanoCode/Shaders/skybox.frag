#version 430

out vec4 fragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    fragColor = texture(skybox, vec3(TexCoords.x, TexCoords.y, TexCoords.z ));
}