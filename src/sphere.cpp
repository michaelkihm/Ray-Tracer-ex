#include "sphere.h"


Sphere::Sphere(vec3 pos, float _radius) :center(pos),radius(_radius){
    radius2 = pow(radius,2);
    normal = vec3(0.0);
}

Sphere::~Sphere(){ }



vec3 Sphere::getNormal(){
     return normal;
}

void Sphere::computeNormal(){
    normal = glm::normalize(Phit - center);     
}

//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
boost::optional<float> Sphere::Intersect(const Ray r)
{
        float t0, t1; // solutions for t if the ray intersects 
        //boost::optional<float> result;
#if 0 
        // geometric solution
        Vec3f L = center - orig; 
        float tca = L.dotProduct(dir); 
        // if (tca < 0) return false;
        float d2 = L.dotProduct(L) - tca * tca; 
        if (d2 > radius2) return false; 
        float thc = sqrt(radius2 - d2); 
        t0 = tca - thc; 
        t1 = tca + thc; 
#else 
        // analytic solution
        vec3 L = r.getOrigin() - center; 
        float a = glm::dot(r.getDirection(),r.getDirection()); 
        float b = 2 * glm::dot(r.getDirection(),L);
        float c = glm::dot(L,L) - radius2;
        if (!solveQuadratic(a, b, c, t0, t1)) 
            return boost::none; 
#endif 
        if (t0 > t1) 
            std::swap(t0, t1); 
 
        if (t0 < 0) { 
            t0 = t1; // if t0 is negative, let's use t1 instead 
            if (t0 < 0) 
                return boost::none; // both t0 and t1 are negative 
        } 
 
        //t = t0;
        //compute normal
        Phit = r.getOrigin() + t0*r.getDirection();
        computeNormal();
 
        return t0; 
    
}

bool Sphere::solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1) 
{ 
    float discr = b * b - 4 * a * c; 
    if (discr < 0) return false; 
    else if (discr == 0) x0 = x1 = - 0.5 * b / a; 
    else { 
        float q = (b > 0) ? 
            -0.5 * (b + sqrt(discr)) : 
            -0.5 * (b - sqrt(discr)); 
        x0 = q / a; 
        x1 = c / q; 
    } 
    if (x0 > x1) std::swap(x0, x1); 
 
    return true; 
} 