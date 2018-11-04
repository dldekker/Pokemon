#version 450 core

/* Shader inputs */
in vec2 vs_uv;

layout(location = 2) uniform float alpha;
uniform sampler2D t;

/* Shader outputs */
out vec4 color;

void main()
{
	color = vec4(texture2D(t, vs_uv).xyz, alpha);
}
