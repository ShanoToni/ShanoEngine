#version 430
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D tex;
uniform int toggle;

void main()
{

	float offset = 1.0 / 300.0;

		vec2 offsets[9] = vec2[](
			vec2(-offset, offset),
			vec2(0.0f, offset),
			vec2(offset, offset),
			vec2(-offset, 0.0f),
			vec2(0.0f, 0.0f),
			vec2(offset, 0.0f),
			vec2(-offset, -offset),
			vec2(0.0f, -offset),
			vec2( offset, -offset)
		);


	//Color invers
	if(toggle==1)
	{
		FragColor = vec4(vec3( 1.0f - texture(tex, TexCoord)), 1.0f);
	} 
	//GREYSCALE
	else if(toggle == 2)
	{
		FragColor = texture(tex, TexCoord);
		float avr = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 *FragColor.b;
		FragColor = vec4(avr, avr, avr, 1.0);
	}
	//SHARPEN KERNEL
	else if(toggle == 3)
	{
		float kernel[9] = float[](
			-1,	-1,	-1,
			-1,	 9,	-1,
			-1,	-1,	-1
		);
		vec3 sampleTex[9];
		for(int i=0; i<9; i++)
		{
			sampleTex[i] = vec3(texture(tex, TexCoord.st+offsets[i]));
		}
		vec3 col = vec3(0.0);
		for(int i=0; i<9; i++)
			col += sampleTex[i] * kernel[i];

		FragColor = vec4(col, 1.0f);
	}
	//BLUR
	else if(toggle == 4)
	{
		float kernel[9] = float[](
			1.0 /16, 	2.0 / 16, 	1.0 / 16,
			2.0 /16, 	4.0 / 16, 	2.0 / 16,
			1.0 /16, 	2.0 / 16, 	1.0 / 16
		);
		vec3 sampleTex[9];
		for(int i=0; i<9; i++)
		{
			sampleTex[i] = vec3(texture(tex, TexCoord.st+offsets[i]));
		}
		vec3 col = vec3(0.0);
		for(int i=0; i<9; i++)
			col += sampleTex[i] * kernel[i];

		FragColor = vec4(col, 1.0f);
	}
	else if(toggle == 5)
	{
		float kernel[9] = float[](
			1, 		1, 		1,
			1, 		-8, 	1,
			1, 		1, 		1

		);
		vec3 sampleTex[9];
		for(int i=0; i<9; i++)
		{
			sampleTex[i] = vec3(texture(tex, TexCoord.st+offsets[i]));
		}
		vec3 col = vec3(0.0);
		for(int i=0; i<9; i++)
			col += sampleTex[i] * kernel[i];

		FragColor = vec4(col, 1.0f);
	}
	//REGULAR
	else
	{
		FragColor = texture(tex, TexCoord);
	}
	
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}