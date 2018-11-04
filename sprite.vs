#version 450 core

layout(location = 0) uniform vec2 screen;
layout(location = 1) uniform vec4 position;

out vec2 vs_uv;

const ivec2 uv[6] = {
	{0, 0}, {0, 1}, {1, 1},
	{0, 0}, {1, 0}, {1, 1}
};

void main()
{
	float x = position[0] + uv[gl_VertexID].x * position[2];
	float y = position[1] + uv[gl_VertexID].y * position[3];

	gl_Position.x = (x / screen.x * 2 - 1);
	gl_Position.y = (1 - y / screen.y * 2);
	gl_Position.z = 0.5;
	gl_Position.w = 1;

	vs_uv = vec2(uv[gl_VertexID].x, uv[gl_VertexID].y);
}
