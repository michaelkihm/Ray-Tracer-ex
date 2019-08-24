#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <boost/optional.hpp>
#include "ray.h"
#include "material.h"

using namespace std;
using namespace glm;

class Primitive{
public:
	//Primitive() {}
	virtual ~Primitive(){};
	virtual vec3 getNormal( ) = 0;

	//material properties
	Material material;
	//char* type;
	//transform
	mat4 transform;

	virtual boost::optional<float> Intersect(const Ray r) = 0;
	virtual void computeNormal() = 0; 
	

protected:
	vec3 Phit;
	vec3 normal;
	//methods
    

	
};

//getNormal




#endif