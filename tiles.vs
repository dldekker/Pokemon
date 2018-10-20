#version 450 core

layout(location = 0) in ivec2 pos;
layout(location = 1) in ivec2 uv;
layout(location = 2) in int tile;

layout(location = 0) uniform vec2 screen;
layout(location = 3) uniform ivec2 cam;
layout(location = 4) uniform float zoom;

out vec2 vs_uv;
flat out int vs_tile;

void main()
{
	float x = pos.x + cam.x;
	float y = pos.y + cam.y;

	gl_Position.x = (x / screen.x * 2) * zoom;
	gl_Position.y = (-(y / screen.y * 2)) * zoom;
	gl_Position.z = 0.5;
	gl_Position.w = 1;

	vs_uv = vec2(uv.x, uv.y);
	vs_tile = tile;
}
