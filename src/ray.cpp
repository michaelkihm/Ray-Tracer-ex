#include "ray.h"


Ray::Ray( const Camera _cam, Size image_size, int i, int j)
:img_size(image_size),cam(_cam)
{
    computeDirectionVector(i,j);
}

// i = row  j= column
void Ray::computeDirectionVector(int i, int j){
    
    float i_half = i + 0.5;
    float j_half = j + 0.5;

    fov_y = cam.getFoV_y();
    origin = cam.getPosition();

    float alpha, beta;

    //compute alpha
    float fovx = (fov_y/2) * img_size.width / img_size.height;
    alpha = glm::tan(glm::radians(fov_x/2))*((j_half - img_size.width*0.5)/img_size.width*0.5);
    assert(alpha <= 1 && alpha >= -1);

    //compute beta
    beta = glm::tan(glm::radians(fov_y/2))*((img_size.height*0.5 - j_half)/img_size.height*0.5);
    assert(beta <= 1 && beta >= -1);

    //compute direction
    direction = glm::normalize(alpha*cam.getU() + beta*cam.getV() - cam.getW());
}