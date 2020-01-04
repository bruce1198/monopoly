#version 410 core


layout(location = 0) in vec3 iv3vertex;
layout(location = 1) in vec3 iv3normal;
layout(location = 2) in vec2 iv2tex_coord;

uniform mat4 position;
uniform mat4 view;
uniform mat4 model;

//uniform mat4 lightSpaceMatrix;

out VertexData
{
  
    vec2 texcoord;
  
	
} vertexData;

uniform vec3 lightPos;

void main()
{
	vec4 worldPosition = model * vec4(iv3vertex, 1.0);
    gl_Position = position * view * worldPosition;

	vertexData.texcoord = iv2tex_coord;
   
											
	
	
    
   
    
    
	
	
}
