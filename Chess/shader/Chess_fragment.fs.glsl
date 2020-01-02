#version 410 core

layout (location = 0) out vec4 color;

uniform vec3 diffuse_albedo = vec3(0.35f, 0.1f, 1.0f); 
uniform vec3 specular_albedo = vec3(0.7);
uniform float specular_power = 200.0;

in VS_OUT
{
	vec3 N;
	vec3 L;
	vec3 V;
} fs_in;

void main(void){
	vec3 N = normalize(fs_in.N);
	vec3 L = normalize(fs_in.L);
	vec3 V = normalize(fs_in.V);
	vec3 H = normalize(L + V);

	vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo;
	vec3 specular = pow(max(dot(N, H), 0.0), specular_power) * specular_albedo;

	color = vec4(diffuse + specular, 1.0);
}
