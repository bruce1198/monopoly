#version 410 core

layout(location = 0) out vec4 fragColor;
in vec2 TexCoords;

uniform sampler2D picture;
uniform float time;

void main()
{
	
	fragColor = vec4(sin((gl_FragCoord.xy + time) / 8), 0.0, 1.0);
	//fragColor = vec4(texture(picture, TexCoords).rgb, 1.0);
	
}