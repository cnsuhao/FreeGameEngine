#ifdef AS_VS

attribute vec4  a_position/*:POSITION*/;
attribute vec2  a_texCoord0/*:TEXCOORD0*/;

uniform mat4	u_matWorldViewProj;

#else //AS_VS

#ifdef GL_ES
	precision mediump float;
#endif
uniform sampler2D u_texture0;
uniform sampler2D u_texture1;

#endif //AS_VS

varying vec2    v_texCoord0;

#ifdef AS_VS

void main()
{
	gl_Position = a_position * u_matWorldViewProj;
	v_texCoord0 = a_texCoord0;
}

#else

uniform float u_progressReference;

void main()
{
	vec4 tex;

	if ( v_texCoord0.x <= u_progressReference )
		tex = texture2D( u_texture1, v_texCoord0 );
	else
		tex = texture2D( u_texture0, v_texCoord0 );
	
	gl_FragColor = tex;
}

#endif
