#ifndef RAY_H
#define RAY_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/mat4x4.hpp"
#include "opencv2/opencv.hpp"
#include "camera.h"
#include <string>
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;
using namespace cv;

class Ray{
    public:
        Ray(){ }
        Ray (const Camera _cam, Size image_size, int i, int j);
        Ray (const glm::vec4 _origin, const glm::vec4 _direction);
        ~Ray(){ }
        inline glm::vec4 getOrigin() const {return origin;}
        inline glm::vec4 getDirection() const {return direction;}

    private:
        glm::vec4 direction;
        glm::vec4 origin;
        float fov_x, fov_y;
        Size img_size;
        Camera cam;

        //methods
        void computeDirectionVector(int i, int j);

};


#endif //RAY_H