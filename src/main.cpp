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

using namespace std;
using namespace cv;

boost::optional< tuple<float, shared_ptr<Primitive>> >IntersectScene(const Ray &r, vector<std::shared_ptr<Primitive>> &scene);
cv::Vec3b FindColor(tuple<float,shared_ptr<Primitive>> hit);

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
  Mat image = Mat::zeros(reader.getImageSize(),CV_8UC3);

  
  //main loop
  for(int i = 0; i < image.rows; i++)
  {
    for(int j=0; j < image.cols; j++)
    {
      Ray r(camera, image.size(), i,j);
      auto hit = IntersectScene(r,scene);
      if(hit)
        image.at<Vec3b>(i,j) = FindColor(*hit);

    }
  }

  imshow("result",image);
  //imwrite("scene1.jpg",image);
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

cv::Vec3b FindColor(tuple<float,shared_ptr<Primitive>> hit){
  auto mat = get<1>(hit)->material;
  return Vec3b(static_cast<uchar>(/*mat.ambient.r**/0), /*static_cast<uchar>(mat.ambient.g*256)*/0,/*static_cast<uchar>(mat.ambient.b*256)*/255);
}