#version 450

uniform sampler2D u_Texture;

in vec3 g_uv;
in vec4 g_color;
in float g_depth;

out vec4 f_FragColor;				

void main()
{
	vec4 texColor = texture2DProj(u_Texture, g_uv);
	float luminosity = (texColor.a + texColor.g + texColor.b) / 3.0;
	gl_FragDepth = g_depth;
    //f_FragColor = vec4(0,0,1,1);
	f_FragColor = mix(texColor, g_color, 1.0 - luminosity) * texColor.a * g_color.a;
}
