#version 450 core

layout(location = 0) uniform vec2 screen;
layout(location = 3) uniform ivec2 cam;
layout(location = 4) uniform float zoom;

layout(location = 5) uniform vec4 player_offset_rect;

out vec2 vs_uv;

const ivec2 uv[6] = {
	{0, 0}, {0, 1}, {1, 1},
	{0, 0}, {1, 0}, {1, 1}
};

void main()
{
	float x = player_offset_rect[0] + player_offset_rect[2] * uv[gl_VertexID].x + cam.x;
	float y = player_offset_rect[1] + player_offset_rect[3] * uv[gl_VertexID].y + cam.y;

	gl_Position.x = (x / screen.x * 2) * zoom;
	gl_Position.y = (-(y / screen.y * 2)) * zoom;
	gl_Position.z = 0.5;
	gl_Position.w = 1;

	vs_uv = vec2(uv[gl_VertexID].x, uv[gl_VertexID].y);
}
