#include <glm/glm.hpp>

using namespace glm;

struct Material{
    vec3 diffuse;	
	vec3 ambient;
	vec3 specular;
	vec3 emission;
	float shininess;
    Material(vec3 _diffuse, vec3 _ambient, vec3 _specular, vec3 _emission, float _shininess)
    : diffuse(_diffuse), ambient(_ambient),specular(_specular), emission(_emission), shininess(_shininess) { }
    Material() {}
Material& operator =(const Material& a)
{
    diffuse = a.diffuse;
    ambient = a.ambient;
    specular = a.specular;
    emission = a.emission;
    shininess = a.shininess;
    return *this;
}

};