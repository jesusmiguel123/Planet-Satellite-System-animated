#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;
out vec2 tc;

uniform mat4 v_matrix;
uniform mat4 p_matrix;
layout (binding = 0) uniform sampler2D samp;

void main(void)
{
	tc = tex_coord;
	mat4 v3_matrix = mat4(mat3(v_matrix));
	gl_Position = p_matrix * v3_matrix * vec4(position,1.0);
}
