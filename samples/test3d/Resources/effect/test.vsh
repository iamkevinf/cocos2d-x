
attribute vec4 a_position;
attribute vec4 a_color;
uniform mat4 u_matWorld;
uniform mat4 u_matViewProj;
varying vec4 v_color;

void main()
{
    gl_Position = u_matWorld * a_position;
    gl_Position = u_matViewProj * a_position;
    v_color = a_color;
}
