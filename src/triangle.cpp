#include "triangle.h"




Triangle::Triangle(vec4 _v1, vec4 _v2, vec4 _v3, mat4 _transform) : A_ut(_v1),B_ut(_v2),C_ut(_v3) 
{
    transform = _transform;
    normal = vec3(0.0f);
    transformVertices();

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

    // Step 1: finding intersection point Q
    computeNormal();
    float NdotRayDirection = glm::dot(normal,ray_direction);
    //check if ray is parallel to plane
    if (fabs(NdotRayDirection) < kEpsilon) // almost 0 
         return boost::none; // they are parallel so they don't intersect !
    //determine t of ray P0 + d*t   -> t = (n*A - n*P0)/n*d
    float t = ( glm::dot(normal,glm::vec3(A.x,A.y,A.z))-glm::dot(normal,ray_origin) )/NdotRayDirection;
    if(t < 0)
        return boost::none; //ray is behind the ray origin
    
    //compute intersection point Q with ray equation
    glm::vec3 Q = ray_origin + t * ray_direction;
    
    // Step 2: is Q inside or outside the triangle

    //check 1st edge BA
    glm::vec3 BA = B - A;
    glm::vec3 QA = Q - A;
    glm::vec3 cross = glm::cross(BA,QA);
    assert(cross.length() == 3);
    if(glm::dot(cross,normal) < 0)
        return boost::none;

    //check 2nd edge CB
    glm::vec3 CB = C - B;
    glm::vec3 QB = Q - B;
    cross = glm::cross(CB,QB);
    if(glm::dot(cross,normal) < 0)
        return boost::none;

    //check 3nd edge AC
    glm::vec3 AC = A - C;
    glm::vec3 QC = Q - C;
    cross = glm::cross(AC,QC);
    if(glm::dot(cross,normal) < 0)
        return boost::none;

    //return t if Q is inside the triangle
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

vec3 Triangle::dehomogenize(vec4 in){
    return vec3(in.x/in.w,in.y/in.w,in.z/in.w);
}


//https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/barycentric-coordinates
// boost::optional<float> Triangle::Intersect(const Ray r)
// {
//     float t;

//     // compute plane's normal
//     computeNormal();
//      glm::vec3 v0v1 = vec3(v1.x,v1.y,v1.z) - vec3(v0.x,v0.y,v0.z); 
//      glm::vec3 v0v2 = vec3(v2.x,v2.y,v2.z) - vec3(v0.x,v0.y,v0.z);
//     // // no need to normalize
//      glm::vec3 normalN = glm::cross(v0v1,v0v2);
//     float denom = glm::dot(normalN,normalN); 
 
//     // Step 1: finding P
    
//     // check if ray and plane are parallel ?
//     float NdotRayDirection = glm::dot(normalN,r.getDirection()); 
//     if (fabs(NdotRayDirection) < kEpsilon) // almost 0 
//         return boost::none; // they are parallel so they don't intersect ! 
 
//     // compute d parameter using equation 2
//     float d = glm::dot(normalN,glm::vec3(v0.x,v0.y,v0.z));
 
//     // compute t (equation 3)
//     t = (glm::dot(normalN,r.getOrigin()) + d) / NdotRayDirection; 
//     // check if the triangle is in behind the ray
//     if (t < 0) 
//         return boost::none; // the triangle is behind 
 
//     // compute the intersection point using equation 1
//     glm::vec3 P = r.getOrigin() + t * r.getDirection(); 
 
//     // Step 2: inside-outside test
//     glm::vec3 C; // vector perpendicular to triangle's plane 
 
//     // edge 0
//     glm::vec3 edge0 = glm::vec3(v1.x,v1.y,v1.z) - glm::vec3(v0.x,v0.y,v0.z); 
//     glm::vec3 vp0 = P - glm::vec3(v0.x,v0.y,v0.z); 
//     C = glm::cross(edge0,vp0 ); 
//     if ( glm::dot(normalN,C) < 0) 
//         return boost::none; // P is on the right side 
 
//     // edge 1
//     glm::vec3 edge1 = glm::vec3(v2.x,v2.y,v2.z) - glm::vec3(v1.x,v1.y,v1.z); 
//     glm::vec3 vp1 = P - glm::vec3(v1.x,v1.y,v1.z); 
//     C = glm::cross(edge1,vp1); 
//     if ( glm::dot(normalN,C) < 0)  
//         return boost::none; // P is on the right side 
 
//     // edge 2
//     glm::vec3 edge2 = glm::vec3(v0.x,v0.y,v0.z) - glm::vec3(v2.x,v2.y,v2.z); 
//     glm::vec3 vp2 = P - glm::vec3(v2.x,v2.y,v2.z); ; 
//     C = glm::cross(edge2, vp2);//.crossProduct(vp2); 
//     if (glm::dot(normalN,C) < 0)    
//         return false; // P is on the right side; 
 
 
//     return t; // this ray hits the triangle 

// }