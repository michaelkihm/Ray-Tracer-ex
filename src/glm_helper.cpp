#include "glm_helper.h"

glm::vec3 dehomogenize(glm::vec4 in){
    return glm::vec3(in.x/in.w,in.y/in.w,in.z/in.w);
}