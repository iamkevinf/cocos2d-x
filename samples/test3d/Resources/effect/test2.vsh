
attribute vec4 a_position;
attribute vec2 a_texCoord0;
uniform mat4 u_matWorldViewProj;
varying vec2 v_texCoord0;

void main()
{
    gl_Position = u_matWorldViewProj * a_position;
    v_texCoord0 = a_texCoord0;
}
