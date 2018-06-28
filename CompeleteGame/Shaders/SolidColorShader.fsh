#ifdef GL_ES
precision lowp float;
#endif

varying vec2 v_texCoord;
uniform int u_color_red;
uniform int u_color_blue;

void main()
{
    vec4 color = texture2D(CC_Texture0, v_texCoord);
    gl_FragColor = vec4(u_color_red, 0, u_color_blue, color.a);
}