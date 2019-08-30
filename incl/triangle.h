#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP


#include "primitive.h"

class Triangle : public Primitive{
public:
	Triangle(vec4 _v1, vec4 _v2, vec4 _v3, mat4 _transform); 
	//Triangle(unsigned int, unsigned int , unsigned int);
	virtual ~Triangle();

	// void applyTransform();
	virtual vec3 getNormal( ) override;
	virtual boost::optional<float> Intersect(const Ray r) override;
	// void setNormal();
	// vec3 getNormal();
private:
	vec3 A,B,C;
	vec4 A_ut,B_ut,C_ut;
	const float kEpsilon = 1e-8;
	
	
    //methods
    virtual void computeNormal() override;
	void transformVertices();
	vec3 dehomogenize(vec4 in);

};




#endif //TRIANGLE_HPP