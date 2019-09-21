#include "sphere.h"


Sphere::Sphere(vec3 pos, float _radius, vec3 _ambient) :center(pos),radius(_radius){
    radius2 = pow(radius,2);
    normal = vec3(0.0);
    ambient = _ambient;
}

Sphere::~Sphere(){ }



vec3 Sphere::getNormal(){
     return normal;
}

void Sphere::computeNormal(){  
    vec4 Papp = vec4(Phit, 1);

	Papp = glm::inverse(transform) * Papp;
    vec3 P3app = dehomogenize(Papp);
	vec3 diff = P3app - center;
	normal =  glm::normalize(diff);

	vec4 normal_4 = vec4(normal.x, normal.y, normal.z, 0);
	normal_4 = glm::transpose(glm::inverse(transform)) * normal_4;
	normal = glm::normalize(vec3(normal_4.x,normal_4.y,normal_4.z));

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
        //intstancing
        vec4 ray_origin = glm::inverse(transform) * r.getOrigin();
        vec4 ray_direction = glm::inverse(transform) * r.getDirection();

        // analytic solution
        vec4 L = ray_origin - vec4(center,1); 
        float a = glm::dot(ray_direction,ray_direction); 
        float b = 2 * glm::dot(ray_direction,L);
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
        //compute normal and hit point
        vec4 P0ht = transform * ray_origin;
		vec4 dHt = transform * ray_direction;
		
		vec4 Pt = P0ht + t0 * dHt;
		Phit = vec3(Pt.x/Pt.w, Pt.y/Pt.w, Pt.z/Pt.w);
        //Phit = r.getOrigin() + t0*r.getDirection();
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

 vec3 Sphere::getPhit(){
     return Phit;
 }
