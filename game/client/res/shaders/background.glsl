#ifdef AS_VS

attribute vec4  a_position/*:POSITION*/;
attribute vec2  a_texCoord0/*:TEXCOORD0*/;

uniform mat4 u_matWorldViewProj;

#else //AS_VS

#ifdef GL_ES
	precision mediump float;
#endif
uniform sampler2D u_texture0;

#endif //AS_VS

varying vec2    v_texCoord0;

#ifdef AS_VS

void main()
{
	vec4 position = a_position * u_matWorldViewProj;
	gl_Position = vec4(a_position.x, a_position.y, position.z / position.w, 1.0);
	v_texCoord0 = a_texCoord0;
}

#else

void main()
{
	gl_FragColor = texture2D(u_texture0, v_texCoord0);
}

#endif
