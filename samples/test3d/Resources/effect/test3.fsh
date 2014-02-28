
uniform sampler2D u_texture0;

varying vec2 v_texCoord0;
varying vec4 v_color;

void main()
{
    gl_FragColor = v_color ;//* texture2D(u_texture0, v_texCoord0);
}