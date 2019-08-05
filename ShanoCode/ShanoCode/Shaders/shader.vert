#version 330

layout (location = 0) in vec3 pos;
uniform mat4 Model;
uniform mat4 Projection;
out vec4 vCol;

void main()
{
	vCol = vec4(clamp(pos,0.0f, 1.0f), 1.0f);
	gl_Position = Projection * Model * vec4(pos.x,pos.y,pos.z, 1.0);

}

