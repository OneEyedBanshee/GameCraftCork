#version 400

in vec4 color;
in vec2 uv;

out vec4 fColor;

uniform sampler2D f_texture;

void main() 
{
	fColor = color - texture2D(f_texture, uv);
}