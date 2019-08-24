#include "camera.h"

Camera::Camera(glm::vec3 eye, glm::vec3 _look_at, glm::vec3 up_vector, float fov)
: eye_position(eye), look_at(_look_at), up(up_vector), field_of_view(fov)
{
	initUpVector();
    initCamera();

}

void Camera::setUpCamera(glm::vec3 eye, glm::vec3 _look_at, glm::vec3 up_vector, float fov)
{
	eye_position = eye;
	look_at = _look_at;
	up_vector = up;
	field_of_view = fov;

	initUpVector();
    initCamera();
}

void Camera::initCamera()
{
    

    glm::vec3 a = look_at - eye_position;
	glm::vec3 b = normalize(up);

	w = glm::normalize(a);
	u = glm::normalize(cross(w,b));
	v = cross(u,w);
	glm::vec3 e = eye_position;
	camera_cs = glm::mat4(u.x,v.x,-1.0 * w.x,0.0,	
			              u.y,v.y,-1.0 * w.y,0.0, 
			              u.z,v.z,-1.0 * w.z,0.0, 
	                      -1.0 * u.x*e.x - u.y*e.y - u.z*e.z, 
	                      -1.0 * v.x*e.x - v.y*e.y - v.z*e.z, 
	                      w.x*e.x + w.y*e.y + w.z*e.z, 1.0);


}

 void Camera::initUpVector()
 {
    glm::vec3 temp = glm::cross(up,eye_position-look_at);
    glm::vec3 up = glm::cross(eye_position-look_at,temp);
 }

