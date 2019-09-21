#include "triangle.h"




Triangle::Triangle(vec4 _v1, vec4 _v2, vec4 _v3, mat4 _transform, vec3 _ambient) : A_ut(_v1),B_ut(_v2),C_ut(_v3) 
{
    transform = _transform;
    normal = vec3(0.0f);
    transformVertices();
    ambient = _ambient;

}


vec3 Triangle::getNormal()
{
    return normal;
}

Triangle::~Triangle()
{

}

void Triangle::computeNormal(){
    glm::vec3 BA = B -A;
    glm::vec3 CA = C - A;
    normal = glm::normalize(glm::cross(BA,CA));
}

//https://courses.cs.washington.edu/courses/csep557/13wi/lectures/triangle_intersection.pdf
boost::optional<float> Triangle::Intersect(const Ray r)
{

    vec3 ray_origin = vec3(r.getOrigin().x,r.getOrigin().y,r.getOrigin().z);
    vec3 ray_direction = vec3(r.getDirection().x,r.getDirection().y,r.getDirection().z);

    // Step 1: finding intersection point Phit
    computeNormal();
    float NdotRayDirection = glm::dot(normal,ray_direction);
    //check if ray is parallel to plane
    if (fabs(NdotRayDirection) < kEpsilon) // almost 0 
         return boost::none; // they are parallel so they don't intersect !
    //determine t of ray P0 + d*t   -> t = (n*A - n*P0)/n*d
    float t = ( glm::dot(normal,glm::vec3(A.x,A.y,A.z))-glm::dot(normal,ray_origin) )/NdotRayDirection;
    if(t < 0)
        return boost::none; //ray is behind the ray origin
    
    //compute intersection point Phit with ray equation
    Phit = ray_origin + t * ray_direction;
    
    // Step 2: is Phit inside or outside the triangle

    //check 1st edge BA
    glm::vec3 BA = B - A;
    glm::vec3 QA = Phit - A;
    glm::vec3 cross = glm::cross(BA,QA);
    assert(cross.length() == 3);
    if(glm::dot(cross,normal) < 0)
        return boost::none;

    //check 2nd edge CB
    glm::vec3 CB = C - B;
    glm::vec3 QB = Phit - B;
    cross = glm::cross(CB,QB);
    if(glm::dot(cross,normal) < 0)
        return boost::none;

    //check 3nd edge AC
    glm::vec3 AC = A - C;
    glm::vec3 QC = Phit - C;
    cross = glm::cross(AC,QC);
    if(glm::dot(cross,normal) < 0)
        return boost::none;

    //return t if Phit is inside the triangle
    return t;

}

void Triangle::transformVertices(){
    A_ut = transform * A_ut;
    B_ut = transform * B_ut;
    C_ut = transform * C_ut;
    A = dehomogenize(A_ut);
    B = dehomogenize(B_ut);
    C = dehomogenize(C_ut);
}


 vec3 Triangle::getPhit(){
     return Phit;
 }
