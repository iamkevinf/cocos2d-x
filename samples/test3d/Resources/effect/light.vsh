
attribute vec4 a_position;
attribute vec3 a_normal;

varying vec4 v_color;

struct DirLight
{
	vec3 dir;
	vec4 color;
};

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emissive;
};

uniform vec4		u_ambient;
uniform DirLight 	u_dirLight;
uniform Material 	u_material;
uniform mat4 		u_matWorld;
uniform mat4		m_matViewProj;

vec4 computeDirLight(vec3 normal)
{
	vec4 color;
	
	float factor = dot(a_dirLight.dir, normal);
	color = factor * u_dirLight.color * u_material.diffuse;
	
	return color;
}

vec4 computeAllLight(vec4 position, vec3 normal)
{
	vec4 color = u_ambient + emissive;
	
	color += computeDirLight(normal);
	return color;
}

void main()
{
	vec4 position = u_matWorld * a_position;
    gl_Position = u_matViewProj * position;
	
	vec3 normal = u_matWorld * a_normal;
    v_color = computeAllLight(position, normal);
}
