/*
    edX Computer Graphics class 
    homework 3
    by M. Kihm 18-Aug-2019
*/

#include <iostream>
#include "readfile.h"
#include "camera.h"
//#include "ray.h"
#include <tuple>
#include <cmath>

using namespace std;
using namespace cv;

const float  eps=1e-4;

boost::optional< tuple<float, shared_ptr<Primitive>> >IntersectScene(const Ray &r, vector<std::shared_ptr<Primitive>> &scene);
cv::Vec3b ComputeLight(tuple<float,shared_ptr<Primitive>> hit, vector<Light> &lights, vector<std::shared_ptr<Primitive>> &scene);
bool IsObjectInShadow(shared_ptr<Primitive> object, const Light &light, vector<std::shared_ptr<Primitive>> &scene, glm::vec3 direction);
//cv::Vec3b ComputeLight(shared_ptr<Primitive> object, const Light &light, const float intersection_position, const bool inShadow);

vec3 light_attenuation;
vec3 eye_pos;

int main(int argc, char* argv[])
{

  if (argc < 2) {
    cerr << "Please provide scene file"<<endl; 
    exit(-1); 
  }
  FileReader reader;
  reader.readfile(argv[1]);
  auto scene = reader.getScene(); 
  auto camera = reader.getCamera();
  auto lights = reader.getLights();
  light_attenuation = reader.getAttenuation();
  Mat image = Mat::zeros(reader.getImageSize(),CV_8UC3);
  eye_pos = camera.getPosition();
  
  //main loop
  for(int i = 0; i < image.rows; i++)
  {
    for(int j=0; j < image.cols; j++)
    {
      Ray r(camera, image.size(), i,j);
      auto hit = IntersectScene(r,scene);
      if(hit)
        image.at<Vec3b>(i,j) = ComputeLight(*hit, lights, scene);

    }
  }

  imshow("result",image);
  imwrite(reader.getFilename(),image);
  cvWaitKey();

  // Mat test = imread("hw3_testscenes/scene1-camera4.jpg");
  // Mat result = image - test;
  // imshow("result",result);
  // cvWaitKey();  
  return 0;
}


boost::optional<tuple<float, shared_ptr<Primitive>> > IntersectScene(const Ray &r, vector<std::shared_ptr<Primitive>> &scene)
{
  int scene_index = -1;
  //boost::optional<float> t;
  float min_t = std::numeric_limits<float>::max();
  shared_ptr<Primitive> object = nullptr;
  //int counter=0;



  for(auto & it : scene)
  {
    auto t = it->Intersect(r);
    if(t) //it there an intersection
    {
      if(*t < min_t)
      {
        min_t = *t;
        object = it;
        //scene_index = counter;
      }
    }
   // counter++;
  }

  if(object == nullptr)
    return boost::none;
  else
  {
    return make_tuple(min_t,object);
  }
  

}

cv::Vec3b ComputeLight(tuple<float,shared_ptr<Primitive>> hit, vector<Light> &lights, vector<std::shared_ptr<Primitive>> &scene){
  auto mat = get<1>(hit)->material;
  auto t = get<0>(hit);
  
  glm::vec3 out_color, light_sum = glm::vec3(0);
  glm::vec3 diffuse_term, specular_term, direction, half_vector, view_direction;
  float distance_r, attenuation;
  glm::vec3 normal = get<1>(hit)->getNormal();

  for(auto c_light : lights)
  {
    glm::vec3 light_pos = glm::vec3(c_light.getPos().x,c_light.getPos().y,c_light.getPos().z);
    glm::vec3 object_pos = get<1>(hit)->getPhit();
    if( c_light.getType() == LightType::point )
    {
      direction = glm::normalize(light_pos-object_pos);
      distance_r = sqrt(glm::dot(object_pos - light_pos, object_pos - light_pos));
      attenuation = 1/(light_attenuation[0]+light_attenuation[1]*distance_r + light_attenuation[2]*pow(distance_r,2));
    }
    else
    {
      direction =glm::normalize( light_pos );
      attenuation = 1;
    }

    if(IsObjectInShadow(get<1>(hit), c_light, scene, direction))
      continue;
    else
    {
  
      glm::vec3 light_attenuated = c_light.getColor()/attenuation;
      diffuse_term = mat.diffuse * light_attenuated * std::max(glm::dot(normal,direction),float(0));
      view_direction = glm::normalize(eye_pos - object_pos);
      half_vector = glm::normalize(direction + view_direction);
      specular_term = mat.specular * light_attenuated * pow(std::max(glm::dot(normal,half_vector),float(0)),mat.shininess);
      
      light_sum += diffuse_term + specular_term;
      
    }


    
  }

  out_color = mat.ambient + mat.emission + light_sum;

  return Vec3b(static_cast<uchar>(out_color.b*255), static_cast<uchar>(out_color.g*255),static_cast<uchar>(out_color.r*255));
}

// --------------------------
//IsObjectInShadow
// -------------------------
bool IsObjectInShadow(shared_ptr<Primitive> object, const Light &light, vector<std::shared_ptr<Primitive>> &scene, glm::vec3 direction)
{

  glm::vec4 ray_direction = glm::vec4(direction,0);
  glm::vec3 offset = direction * eps;
  glm::vec3 temp_ray_origin = object->getPhit() + offset;
  glm::vec4 ray_origin = glm::vec4(temp_ray_origin,1);
  Ray ray(ray_origin,ray_direction);

  //iterate through scene and check for intersection between ray and light
  for(auto & it : scene)
  {
    auto t = it->Intersect(ray);
    if(t) //it there an intersection
      return true;
  }

  return false;
}

// cv::Vec3b ComputeLight(shared_ptr<Primitive> object, const Light &light, const float intersection_position, const bool inShadow)
// {



//   return vec3(0,0,0);
// }
 //vec4 ComputeLight (const in vec3 direction, const in vec4 lightcolor, const in vec3 normal, const in vec3 halfvec, const in vec4 mydiffuse, const in vec4 myspecular, const in float myshininess)