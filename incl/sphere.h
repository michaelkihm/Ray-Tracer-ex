#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "primitive.h"
#include <cmath>
#include <algorithm>


class Sphere : public Primitive{
public:
	Sphere(vec3 pos, float _radius);
	//Sphere() { }
	virtual	~Sphere();
    virtual vec3 getNormal( ) override;// { return normal;}
	virtual boost::optional<float> Intersect(const Ray r) override;
private:
	const vec3 center;
	const float radius;
	float radius2;
	


	//methods
	virtual void computeNormal() override;
	bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);
	

};



#endif //SPHERE_HPPs