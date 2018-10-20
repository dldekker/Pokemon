#version 450 core

/* Shader inputs */
in vec2 vs_uv;

/* Uniform inputs */
layout(location = 1) uniform int tiles_x;
layout(location = 2) uniform int tiles_y;

layout(location = 6) uniform vec4 clip_rect;

uniform sampler2D player_texture;

/* Shader outputs */
out vec4 color;

void main()
{
	// Scale the vs_uv to be the size of a tile.
	float uv_x = clip_rect[0] / 400.0 + vs_uv.x / 400.0 * (clip_rect[2]);
	float uv_y = clip_rect[1] / 228.0 + vs_uv.y / 228.0 * (clip_rect[3]);

	color = texture(player_texture, vec2(uv_x, uv_y));
}
