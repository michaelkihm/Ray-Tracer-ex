#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP


#include "primitive.h"
#include "glm_helper.h"

class Triangle : public Primitive{
public:
	Triangle(vec4 _v1, vec4 _v2, vec4 _v3, mat4 _transform, vec3 _ambient); 

	virtual ~Triangle();
	virtual vec3 getNormal( ) override;
	virtual boost::optional<float> Intersect(const Ray r) override;
	virtual vec3 getPhit() override;

private:
	vec3 A,B,C;
	vec4 A_ut,B_ut,C_ut;
	const float kEpsilon = 1e-8;
	
	
    //methods
    virtual void computeNormal() override;
	void transformVertices();

};




#endif //TRIANGLE_HPP