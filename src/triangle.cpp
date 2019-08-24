#include "triangle.h"


Triangle::Triangle(vec4 _v1, vec4 _v2, vec4 _v3) : v0(_v1),v1(_v2),v2(_v3)
{
    normal = vec3(0.0f);

}


vec3 Triangle::getNormal()
{
    return normal;
}

Triangle::~Triangle()
{

}

void Triangle::computeNormal(){
    glm::vec3 v0v1 = vec3(v1.x,v1.y,v1.z) - vec3(v0.x,v0.y,v0.z); 
    glm::vec3 v0v2 = vec3(v2.x,v2.y,v2.z) - vec3(v0.x,v0.y,v0.z); 
    normal = glm::normalize( glm::cross(v0v1,v0v2));
}

//https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/barycentric-coordinates
boost::optional<float> Triangle::Intersect(const Ray r)
{
    float t;

    // compute plane's normal
    computeNormal();
    // glm::vec3 v0v1 = vec3(v1.x,v1.y,v1.z) - vec3(v0.x,v0.y,v0.z); 
    // glm::vec3 v0v2 = vec3(v2.x,v2.y,v2.z) - vec3(v0.x,v0.y,v0.z);
    // // no need to normalize
    // normal = glm::cross(v0v1,v0v2);
    float denom = glm::dot(normal,normal); 
 
    // Step 1: finding P
 
    // check if ray and plane are parallel ?
    float NdotRayDirection = glm::dot(normal,r.getDirection()); 
    if (fabs(NdotRayDirection) < kEpsilon) // almost 0 
        return boost::none; // they are parallel so they don't intersect ! 
 
    // compute d parameter using equation 2
    float d = glm::dot(normal,glm::vec3(v0.x,v0.y,v0.z));
 
    // compute t (equation 3)
    t = (glm::dot(normal,r.getOrigin()) + d) / NdotRayDirection; 
    // check if the triangle is in behind the ray
    if (t < 0) return boost::none; // the triangle is behind 
 
    // compute the intersection point using equation 1
    glm::vec3 P = r.getOrigin() + t * r.getDirection(); 
 
    // Step 2: inside-outside test
    glm::vec3 C; // vector perpendicular to triangle's plane 
 
    // edge 0
    glm::vec3 edge0 = glm::vec3(v1.x,v1.y,v1.z) - glm::vec3(v0.x,v0.y,v0.z); 
    glm::vec3 vp0 = P - glm::vec3(v0.x,v0.y,v0.z); 
    C = glm::cross(edge0,vp0 ); 
    if ( glm::dot(normal,C) < 0) return boost::none; // P is on the right side 
 
    // edge 1
    glm::vec3 edge1 = glm::vec3(v2.x,v2.y,v2.z) - glm::vec3(v1.x,v1.y,v1.z); 
    glm::vec3 vp1 = P - glm::vec3(v1.x,v1.y,v1.z); 
    C = glm::cross(edge1,vp1); 
    if ( glm::dot(normal,C) < 0)  return boost::none; // P is on the right side 
 
    // edge 2
    glm::vec3 edge2 = glm::vec3(v0.x,v0.y,v0.z) - glm::vec3(v2.x,v2.y,v2.z); 
    glm::vec3 vp2 = P - glm::vec3(v2.x,v2.y,v2.z); ; 
    C = glm::cross(edge2, vp2);//.crossProduct(vp2); 
    if (glm::dot(normal,C) < 0) return false; // P is on the right side; 
 
 
    return t; // this ray hits the triangle 

}