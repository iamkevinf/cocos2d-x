
attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord0;

varying vec4 v_color;
varying vec2 v_texCoord0;

uniform mat4 		u_matWorld;
uniform mat4		u_matViewProj;
uniform vec4		u_ambient;

struct DirLight
{
    vec4  direction;
    vec4  color;
    vec3  halfVector;
};

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 emissive;
};

uniform DirLight u_dirLight;
uniform Material u_material;

vec4 computeDirLight(vec3 normal)
{
	vec4 color;
	float factor;
    factor = max(0.0, dot(u_dirLight.direction.xyz, normal));
	color = u_dirLight.color * u_material.diffuse * factor;
    
    factor = dot(u_dirLight.halfVector, normal);
    if(factor > 0.0)
    {
        color += pow(factor, 0.5) * u_dirLight.color * u_material.specular;
    }
	
	return color;
}

vec4 computeAllLight(vec3 position, vec3 normal)
{
	vec4 color = u_material.emissive;
    
    color += u_ambient * u_material.ambient;
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
	
	normal = normalize( (u_matWorld * vec4( a_normal, 0 )).xyz );
    
    //if(u_dirLight.direction.w > 0.0)
        v_color = computeAllLight(position.xyz, normal);
    
    v_texCoord0 = a_texCoord0;
}
