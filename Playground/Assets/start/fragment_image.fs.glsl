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
	
	if (texColor.a < 0.1)
		discard;
	//fragColor = texture(tex,vertexData.texcoord);
	//texColor.r = max(texColor.r -time/255.0, 0);
	
	fragColor = vec4(texColor.rgb, 1.0);

}




