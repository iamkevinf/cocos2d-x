
attribute vec4 a_position;
attribute vec4 a_normal;
attribute vec2 a_texCoord0;

varying vec4 v_color;
varying vec2 v_texCoord0;

struct DirLight
{
	vec4 dir;
	vec4 color;
};

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emissive;
};

uniform mat4 		u_matWorld;
uniform mat4		u_matViewProj;
uniform vec4		u_ambient;

uniform bool        u_dirLight;
uniform vec3        u_dirLightDir;
uniform vec4        u_dirLightColor;
uniform Material 	u_material;


vec4 computeDirLight(vec3 normal)
{
	vec4 color;
	
	float factor = dot(u_dirLightDir, normal);
	color = factor * u_dirLightColor * vec4(1.0, 1.0, 1.0, 1.0);
	
	return color;
}

vec4 computeAllLight(vec3 position, vec3 normal)
{
	vec4 color = u_ambient;// + u_material.emissive;
	
	color += computeDirLight(normal);
    color.w = 1.0;
	return color;
}

void main()
{
    vec3 normal;
	vec4 position;
    
    position= u_matWorld * a_position;
    gl_Position = u_matViewProj * position;
	
	normal = normalize( (u_matWorld * a_normal).xyz );
    
    v_color = computeAllLight(position.xyz, normal);
    v_texCoord0 = a_texCoord0;
}
