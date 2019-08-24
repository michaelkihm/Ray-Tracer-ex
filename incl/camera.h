#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/mat4x4.hpp"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Camera{
    public:
        Camera() {}
        Camera(glm::vec3 eye, glm::vec3 _look_at, glm::vec3 up_vector, float fov);
        ~Camera() {}
        inline glm::vec3 getPosition() {return eye_position; }
        inline glm::vec3 getU() {return u; }
        inline glm::vec3 getV() {return v; }
        inline glm::vec3 getW() {return w; }
        inline glm::mat4 getCS() {return camera_cs;}
        inline float getFoV_y() const {return field_of_view;}
        void setUpCamera(glm::vec3 eye, glm::vec3 _look_at, glm::vec3 up_vector, float fov);

    private:
        glm::vec3 eye_position;
        glm::vec3 up, look_at;
        glm::vec3 u,v,w;
        glm::mat4 camera_cs;
        float field_of_view;

        //methods
        void initCamera();
        void initUpVector();



};


#endif //CAMERA_H