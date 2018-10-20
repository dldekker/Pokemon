#version 450 core

/* Shader inputs */
in vec2 vs_uv;
flat in int vs_tile;

/* Uniform inputs */
layout(location = 1) uniform int tiles_x;
layout(location = 2) uniform int tiles_y;

uniform sampler2D tileset;

/* Shader outputs */
out vec4 color;

void main()
{
	// Scale the vs_uv to be the size of a tile.
	float uv_x = vs_uv.x / tiles_x;
	float uv_y = vs_uv.y / tiles_y;

	// Offset the position to the right tile.
	int tile_offset_x = vs_tile % 88;
	int tile_offset_y = (vs_tile - tile_offset_x) / 69;
	uv_x += float(tile_offset_x) / tiles_x;
	uv_y += float(tile_offset_y) / tiles_y;

	color = texture(tileset, vec2(uv_x, uv_y));
}
