#include "ray.h"


Ray::Ray( const Camera _cam, Size image_size, int i, int j)
:img_size(image_size),cam(_cam)
{
    computeDirectionVector(i,j);
    origin = glm::vec4(cam.getPosition(),1);
}

// i = row  j= column
void Ray::computeDirectionVector(int i, int j){
    
    float i_half = i + 0.5;
    float j_half = j + 0.5;

    fov_y = cam.getFoV_y();
    //float tan_fovy_2 = glm::tan(glm::radians(fov_y)*0.5);
   

    float alpha, beta;
    float img_width = static_cast<float>(img_size.width);
    float img_height = static_cast<float>(img_size.height);
      float imageAspectRatio = img_width / (float)img_height;

    //compute alpha
    //float tan_fovx_2 = tan_fovy_2 * img_width / img_height;
    //alpha = tan_fovx_2*((j_half - img_width*0.5)/img_width*0.5);
    alpha = (2 * ((j + 0.5) / img_width) - 1) * glm::tan(fov_y / 2 * M_PI / 180) * imageAspectRatio;
    assert(alpha <= 1 && alpha >= -1);

    //compute beta
    //beta = tan_fovy_2*((img_height*0.5 - i_half)/img_height*0.5);
    beta = (1 - 2 * ((i + 0.5) / img_height)) * glm::tan(fov_y / 2 * M_PI / 180);
    assert(beta <= 1 && beta >= -1);

   
    //compute direction
    glm::vec3 temp_direction = glm::normalize(alpha*cam.getU() + beta*cam.getV() - cam.getW());
    direction = glm::vec4(temp_direction,0);

    //test stuff
  
//     float Px = (2 * ((j + 0.5) / img_width) - 1) * glm::tan(fov_y / 2 * M_PI / 180) * imageAspectRatio; 
//    float Py = (1 - 2 * ((i + 0.5) / img_height)) * glm::tan(fov_y / 2 * M_PI / 180);
//     glm::vec4 origin = glm::vec4(cam.getPosition().x, cam.getPosition().y, cam.getPosition().z,1);
//     glm::vec4 t_direction = glm::vec4(Px,Py,-1,1); 
//     glm::vec4 test_dir = glm::normalize(cam.getCS()*(t_direction -origin));
//     test_dir = glm::normalize(test_dir);
 
}
