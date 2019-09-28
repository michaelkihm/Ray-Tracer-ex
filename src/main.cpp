/*
    edX Computer Graphics class 
    homework 3
    by M. Kihm 18-Aug-2019
*/

#include <iostream>
#include "readfile.h"
#include "camera.h"
#include <tuple>
#include <cmath>
#include <boost/optional/optional_io.hpp>

using namespace std;
using namespace cv;

const float  eps=1e-4;
const uint color_channels = 3;

boost::optional< tuple<float, shared_ptr<Primitive>,vec3> >IntersectScene(const Ray &r, vector<std::shared_ptr<Primitive>> &scene);
glm::vec3 ComputeLight(tuple< float,shared_ptr<Primitive>, vec3 > hit, glm::vec3 eye_pos, int depth);
bool IsObjectInShadow(shared_ptr<Primitive> object, const Light &light, glm::vec3 direction, float distance, float r_obj_light);
glm::vec3 getSpecularDirection(glm::vec3 rayDirection, glm::vec3 normal);

vec3 light_attenuation;
vector<Light> lights;
vector<shared_ptr<Primitive>> scene;

int main(int argc, char* argv[])
{

  if (argc < 2) {
    cerr << "Please provide scene file"<<endl; 
    exit(-1); 
  }
  FileReader reader;
  reader.readfile(argv[1]);
  scene = reader.getScene(); 
  lights = reader.getLights();
  auto camera = reader.getCamera();
  light_attenuation = reader.getAttenuation();
  Mat image = Mat::zeros(reader.getImageSize(),CV_8UC3);
  vec3 eye_pos = camera.getPosition();
  int maxdepth = reader.getMaxDepth();
  
  //main loop
  for(int i = 0; i < image.rows; i++)
  {
    for(int j=0; j < image.cols; j++)
    {
      Ray r(camera, image.size(), i,j);
      auto hit = IntersectScene(r,scene);
      if(hit){
        glm::vec3 color = ComputeLight(*hit, eye_pos, maxdepth);
        image.at<Vec3b>(i,j) = Vec3b(static_cast<uchar>(color.b*255), static_cast<uchar>(color.g*255),static_cast<uchar>(color.r*255));
      }

    }
  }

  imshow("result",image);
  imwrite(reader.getFilename(),image);
  cvWaitKey(); 
  return 0;
}


boost::optional< tuple<float, shared_ptr<Primitive>,vec3> > IntersectScene(const Ray &r, vector<std::shared_ptr<Primitive>> &scene)
{
  int scene_index = -1;
  float min_t = std::numeric_limits<float>::max();
  shared_ptr<Primitive> object = nullptr;



  for(auto & it : scene)
  {
    auto t = it->Intersect(r);
    if(t) //is there an intersection
    {
      if(t < min_t && *t > 0)
      {
        min_t = *t;
        object = it;
      }
    }
  }

  if(object == nullptr)
    return boost::none;
  else
  {
    return make_tuple(min_t,object,vec3(r.getDirection().x,r.getDirection().y,r.getDirection().z));
  }
  

}

glm::vec3 ComputeLight(tuple<float,shared_ptr<Primitive>,vec3> hit, vec3 eye_pos, int depth){
  auto hit_primitive = get<1>(hit);
  auto mat = hit_primitive->material;
  auto t = get<0>(hit);
  
  glm::vec3 out_color=glm::vec3(0);
  glm::vec3 diffuse_term, specular_term, direction, half_vector, view_direction;
  float distance_obj_light, attenuation;
  glm::vec3 normal = hit_primitive->getNormal();
  glm::vec3 object_pos = hit_primitive->getPhit();
  if(depth){
    for(auto c_light : lights)
    {
      glm::vec3 light_pos = glm::vec3(c_light.getPos().x,c_light.getPos().y,c_light.getPos().z);
      distance_obj_light = sqrt(glm::dot(object_pos - light_pos, object_pos - light_pos));
      if( c_light.getType() == LightType::point )
      {
        direction = glm::normalize(light_pos-object_pos);
        attenuation = 1/(light_attenuation[0]+light_attenuation[1]*distance_obj_light + light_attenuation[2]*pow(distance_obj_light,2));
      }
      else
      {
        direction =glm::normalize( light_pos );
        attenuation = 1;
      }

      if(IsObjectInShadow(hit_primitive, c_light, direction,t,distance_obj_light))
        continue;
      else
      {
        //diffuse
        glm::vec3 light_attenuated = c_light.getColor()/attenuation;
        diffuse_term = mat.diffuse * light_attenuated * std::max(glm::dot(normal,direction),float(0));

        //specular
        view_direction = glm::normalize(eye_pos - object_pos);
        half_vector = glm::normalize(direction + view_direction);
        specular_term = mat.specular * light_attenuated * pow(std::max(glm::dot(normal,half_vector),float(0)),mat.shininess);
        
        out_color += diffuse_term + specular_term;
        
      }


      
    }

    out_color += mat.ambient + mat.emission;

    //recursive mirror term
    vec3 new_direction = getSpecularDirection(get<2>(hit) ,normal);
    vec3 offset = eps * new_direction;
    vec3 new_origin = object_pos + offset;
    Ray new_ray(vec4(new_origin,1),vec4(new_direction,0));
    auto new_hit = IntersectScene(new_ray,scene);
    if(new_hit){
      vec3 add = ComputeLight(*new_hit,object_pos,depth-1);
      out_color += mat.specular * add;
    }
  }

  for(uint i = 0; i < color_channels; i++)
  {
    if(out_color[i] > 1) out_color[i]=1;
    else if(out_color[i] < 0) out_color[i]=0;
  }

  return out_color;
}

// --------------------------
//IsObjectInShadow
// -------------------------
bool IsObjectInShadow(shared_ptr<Primitive> object, const Light &light, glm::vec3 direction, float distance, float r_obj_light)
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
    if(t){ //it there an intersection
      if(*t < r_obj_light && *t > 0)
        return true;
    }
  }

  return false;
}


glm::vec3 getSpecularDirection(glm::vec3 rayDirection, glm::vec3 normal){
	//r = d − 2(d · n)n
  return glm::normalize(rayDirection - 2*glm::dot(rayDirection,normal)*normal);
}