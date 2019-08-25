#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "primitive.h"

class Triangle : public Primitive{
public:
	Triangle(vec4 _v1, vec4 _v2, vec4 _v3); 
	//Triangle(unsigned int, unsigned int , unsigned int);
	virtual ~Triangle();

	// void applyTransform();
	virtual vec3 getNormal( ) override;
	virtual boost::optional<float> Intersect(const Ray r) override;
	// void setNormal();
	// vec3 getNormal();
private:
	const vec4 A,B,C;
	const float kEpsilon = 1e-8;

    //methods
    virtual void computeNormal() override;
	

};




#endif //TRIANGLE_HPP