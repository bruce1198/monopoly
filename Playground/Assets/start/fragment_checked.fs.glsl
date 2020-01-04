#version 410 core

layout(location = 0) out vec4 fragColor;

void main()
{
	//TODO:
	//Please modify the fragment shader (Checked)
	if(	(mod(gl_FragCoord.x, 30) < 15 && mod(gl_FragCoord.y, 20) < 10) || 
		(mod(gl_FragCoord.x, 30) > 15 && mod(gl_FragCoord.y, 20) > 10))
	{
	    fragColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
	else
	{
	    fragColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
    //////////
}