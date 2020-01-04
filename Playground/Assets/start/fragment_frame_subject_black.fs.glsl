#version 410

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D origin;
uniform sampler2D darker;


void main()
{ 
	vec4 color = texture(darker, TexCoords);	
	FragColor = vec4(color.rgb,1.0);		
		
}


