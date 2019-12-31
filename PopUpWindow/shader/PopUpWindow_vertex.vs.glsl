#version 410 core

layout(location = 0) in vec4 vertex;
layout(location = 1) in vec2 tex_coord;

out VS_OUT{
	vec2 texcoord;
} vs_out;

uniform mat4 mvp;

void main(void){
	gl_Position = mvp * vertex;
	vs_out.texcoord = tex_coord;
}