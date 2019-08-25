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
	up = up_vector;
	field_of_view = fov;

	initUpVector();
    initCamera();
}

void Camera::initCamera()
{
    
	//glm::mat4 test = glm::lookAt(eye_position,look_at,up);

    glm::vec3 a =  eye_position - look_at;
	glm::vec3 b = up;//normalize(up);

	w = glm::normalize(a);
	u = glm::normalize(cross(b,w));
	v = cross(w,u);
	//glm::vec3 e = eye_position;
	camera_cs =  glm::lookAt(eye_position,look_at,up);
	
	// camera_cs = glm::mat4(u.x,v.x,-1.0 * w.x,0.0,	
	// 		              u.y,v.y,-1.0 * w.y,0.0, 
	// 		              u.z,v.z,-1.0 * w.z,0.0, 
	//                       -1.0 * u.x*e.x - u.y*e.y - u.z*e.z, 
	//                       -1.0 * v.x*e.x - v.y*e.y - v.z*e.z, 
	//                       w.x*e.x + w.y*e.y + w.z*e.z, 1.0);

	// printMatFour(test);
	// cout << "-----------"<< endl;
	// printMatFour(camera_cs);
	//assert(test == camera_cs);

}


 void Camera::initUpVector()
 {
    glm::vec3 x = glm::cross(up,eye_position-look_at);
    glm::vec3 y = glm::cross(eye_position-look_at,x);
	up = glm::normalize(y);
 }

void Camera::printMatFour(glm::mat4 &matrix)
{

	for(int r=0; r < 4; r++)
	{
		for(int c=0; c < 4; c++)
			std::cout << matrix[r][c]<<" ";
		std::cout << "\n";
	}

}