// Readfile definitions 
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/mat4x4.hpp" 

// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <vector>
#include <memory>
#include <boost/ptr_container/ptr_vector.hpp>


//#include <glm/gtc/matrix_transform.hpp> 
#include "primitive.h"
#include "sphere.h"
#include "triangle.h"
#include "light.h"
#include "camera.h"

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;
using namespace glm;

//#include "variables.h" 

using namespace std;

class FileReader
{
    public:
        FileReader();
        ~FileReader(){ }
        Camera getCamera();
        vector<std::shared_ptr<Primitive>> getScene();
        cv::Size getImageSize();
        void readfile (const char * filename);
        vector<Light> getLights() { return lights; }
        vec3 getAttenuation() { return attenuation; }
        string getFilename() {return out_filename; }

    private:
        int maxdepth = 5;
        int image_height;
        int image_width;
        Camera camera;
        vec3 eye_position;
        vec3 camera_up_vector;
        vec3 camera_lookat;
        float field_of_view;
        vector<vec4> vertices;
        vector<std::shared_ptr<Primitive>> scene;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        vec4 emission;
        float shininess;
        vector<Light> lights;
        string out_filename;
        vec3 attenuation;

        void matransform (stack<glm::mat4> &transfstack, float * values) ;
        void rightmultiply (const glm::mat4 & M, stack<glm::mat4> &transfstack) ;
        //bool readvals (stringstream &s, const int numvals, float * values) ;
        bool readvalsf(stringstream &s, const int numvals, float* values); 
        bool readvalsi(stringstream &s, const int numvals, int* values);
        


};
