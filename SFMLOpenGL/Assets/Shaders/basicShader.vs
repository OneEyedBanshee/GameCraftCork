#version 400
		
in vec3 sv_position;
in vec4 sv_color;
in vec2 sv_uv;
	
out vec4 color;
out vec2 uv;
		
uniform mat4 sv_mvp;
uniform float sv_x_offset;
uniform float sv_y_offset;
uniform float sv_z_offset;

void main() 
{
	color = sv_color;
	uv = sv_uv;
	gl_Position = sv_mvp * vec4(sv_position.x + sv_x_offset, sv_position.y + sv_y_offset, sv_position.z + sv_z_offset, 1 );
}