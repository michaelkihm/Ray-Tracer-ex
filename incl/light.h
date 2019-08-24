#ifndef LIGHTS
#define LIGHTS
#include "glm/glm.hpp"

enum class LightType {point, directional};

class Light
{
       public:
        Light(){ }
        Light(LightType _type, glm::vec4 _position, glm::vec3 _color);
        //:position(_position), color(_color) { }           
        ~Light() {}
        glm::vec4 getPos() {return position;}
        glm::vec3 getColor() {return color; }

    private:
        glm::vec4 position;
        glm::vec3 color;
        LightType type;
};


#endif //LIGHTS