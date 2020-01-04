#version 410

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

//uniform float my_time;
//uniform int frame_flag;
//uniform int frame_mode;
//uniform float mouse_placeX;
//uniform float mouse_placeY;



void main()
{ 
	FragColor = texture(screenTexture, TexCoords);			
		
}


