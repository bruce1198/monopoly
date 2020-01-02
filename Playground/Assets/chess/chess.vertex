#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 mv_matrix;
uniform mat4 p_matrix;

uniform vec3 lightPos = vec3(50.0f, 50.0f, -50.0f);

out VS_OUT
{
	vec3 N;
	vec3 L;
	vec3 V;
} vs_out;

void main(void){

	vec4 P = mv_matrix * vec4(position, 1.0);
	vs_out.N = mat3(mv_matrix) * normal;
	vs_out.L = mat3(mv_matrix) * lightPos - P.xyz;
	vs_out.V = -P.xyz;

	gl_Position = p_matrix * mv_matrix * vec4(position, 1.0);
}
