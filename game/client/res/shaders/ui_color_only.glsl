
#ifdef AS_VS

attribute vec4 	a_position/*:POSITION*/;
attribute vec4 	a_color/*:COLOR*/;

uniform mat4 u_matWorldViewProj;

#else //AS_PS

#ifdef GL_ES
	precision mediump float;
#endif

#endif

varying vec4 v_color;

#ifdef AS_VS

void main()
{
	gl_Position = a_position * u_matWorldViewProj; 
	v_color = a_color.bgra;
}

#else

void main()
{
	gl_FragColor = v_color;
}

#endif
