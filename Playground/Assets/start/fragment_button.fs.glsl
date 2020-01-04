#version 410 core

out vec4 FragColor;
layout(location = 0) out vec4 fragColor;


in VertexData
{
   // vec3 N; // eye space normal
   // vec3 L; // eye space light vector
   // vec3 H; // eye space halfway vector
    vec2 texcoord;
} vertexData;

uniform sampler2D tex;
uniform float time;
void main()
{
	vec4 texColor = texture(tex,vertexData.texcoord);
	//texColor.r = max(texColor.r- sin(time),0);
	//texColor.r = sin(time);
	if (texColor.a < 0.1)
		discard;
	fragColor = texColor;
	
}




