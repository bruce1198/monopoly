#version 410 core

uniform sampler2D tex;
// uniform vec2 texOffset = vec2(-400, -159);

out vec4 color;

in VS_OUT{
	vec2 texcoord;
} fs_in;

void main(void){
	// color = vec4(1.0, 0.0, 0.0, 1.0);
	// color = texelFetch(tex, ivec2(gl_FragCoord.xy) + ivec2(texOffset), 0);
	// color = vec4(texture(tex, fs_in.texcoord).rgb, 1.0);
	color = texture(tex, fs_in.texcoord);
}