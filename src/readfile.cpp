/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

/*****************************************************************************/
// This file is readfile.cpp.  It includes helper functions for matrix 
// transformations for a stack (matransform) and to rightmultiply the 
// top of a stack.  These functions are given to aid in setting up the 
// transformations properly, and to use glm functions in the right way.  
// Their use is optional in your program.  


// The functions readvals and readfile do basic parsing.  You can of course 
// rewrite the parser as you wish, but we think this basic form might be 
// useful to you.  It is a very simple parser.

// Please fill in parts that say YOUR CODE FOR HW 2 HERE. 
// Read the other parts to get a context of what is going on. 

/*****************************************************************************/




//using namespace glm;

#include "readfile.h"


// You may not need to use the following two functions, but it is provided
// here for convenience




// The function below applies the appropriate transform to a 4-vector
void FileReader::matransform(stack<glm::mat4> &transfstack, float* values) 
{
  mat4 transform = transfstack.top(); 
  vec4 valvec = vec4(values[0],values[1],values[2],values[3]); 
  vec4 newval = transform * valvec; 
  for (int i = 0; i < 4; i++) values[i] = newval[i]; 
}

void FileReader::rightmultiply(const mat4 & M, stack<mat4> &transfstack) 
{
  mat4 &T = transfstack.top(); 
  T = T * M; 
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool FileReader::readvalsf(stringstream &s, const int numvals, float* values) 
{
  for (int i = 0; i < numvals; i++) {
    s >> values[i]; 
    if (s.fail()) {
      cout << "Failed reading value " << i << " will skip\n"; 
      return false;
    }
  }
  return true; 
}

bool FileReader::readvalsi(stringstream &s, const int numvals, int* values) 
{
  for (int i = 0; i < numvals; i++) {
    s >> values[i]; 
    if (s.fail()) {
      cout << "Failed reading value " << i << " will skip\n"; 
      return false;
    }
  }
  return true; 
}

void FileReader::readfile(const char* filename) 
{
  string str, cmd; 
  ifstream in;
  in.open(filename); 
  if (in.is_open()) {

    // I need to implement a matrix stack to store transforms.  
    // This is done using standard STL Templates 
    stack <mat4> transfstack; 
    transfstack.push(mat4(1.0));  // identity

    getline (in, str); 
    while (in) {
      if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
        // Ruled out comment and blank lines 

        stringstream s(str);
        s >> cmd; 
        int i; 
        float values[10]; // Position and color for light, colors for others
        int values_i[10];
        // Up to 10 params for cameras.  
        bool validinput; // Validity of input 

        // Process the light, add it to database.
        // Lighting Command
        if (cmd == "directional" || cmd == "point") {
              validinput = readvalsf(s, 6, values);
              if(validinput)
              {
                glm::vec4 position(values[0],values[1],values[2],1.0);
                glm::vec3 color(values[3],values[4],values[5]);
                LightType type;
                if(cmd == "directional")
                  type = LightType::directional;
                else
                  type = LightType::point;
                lights.push_back(Light(type,position, color));
                
              }          
            }
        

        // Material Commands 
        // Ambient, diffuse, specular, shininess properties for each object.
        // Filling this in is pretty straightforward, so I've left it in 
        // the skeleton, also as a hint of how to do the more complex ones.
        // Note that no transforms/stacks are applied to the colors. 

        else if (cmd == "ambient") {
          validinput = readvalsf(s, 3, values); // colors 
          if (validinput) {
            for (i = 0; i < 3; i++) {
              ambient[i] = values[i]; 
            }
          }
        } else if (cmd == "diffuse") {
          validinput = readvalsf(s, 3, values); 
          if (validinput) {
            for (i = 0; i < 3; i++) {
              diffuse[i] = values[i]; 
            }
          }
        } else if (cmd == "specular") {
          validinput = readvalsf(s, 3, values); 
          if (validinput) {
            for (i = 0; i < 3; i++) {
              specular[i] = values[i]; 
            }
          }
        } else if (cmd == "emission") {
          validinput = readvalsf(s, 3, values); 
          if (validinput) {
            for (i = 0; i < 3; i++) {
              emission[i] = values[i]; 
            }
          }
        } else if (cmd == "shininess") {
          validinput = readvalsf(s, 1, values); 
          if (validinput) {
            shininess = values[0]; 
          }
        } else if (cmd == "size") {
          validinput = readvalsf(s,2,values); 
          if (validinput) { 
            image_width = static_cast<int>(values[0]); 
            image_height = static_cast<int>(values[1]); 
          } 
        } else if (cmd == "camera") {
          validinput = readvalsf(s,10,values); // 10 values eye cen up fov
          if (validinput) {

            //camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fovy
            eye_position = vec3(values[0],values[1],values[2]);
            camera_lookat = vec3(values[3],values[4],values[5]);
            //upinit = Transform::upvector(vec3(values[6], values[7], values[8]), eyeinit-center);
            camera_up_vector = vec3(values[6],values[7],values[8]);
						field_of_view = values[9];
            camera.setUpCamera(eye_position, camera_lookat,camera_up_vector, field_of_view);
        

          }
        }
        else if(cmd == "maxdepth"){ //max number of bounces for a ray
          validinput = readvalsi(s,1,values_i);
          if(validinput)
            maxdepth = values[0];
        }
        else if (cmd == "output"){
					out_filename = (char*) malloc(100);
					s >> out_filename;
        }
        else if(cmd == "vertex"){
          validinput = readvalsf(s,3,values); // 
					if (validinput) {
						vec4 vertex = vec4(values[0],values[1],values[2],1.0);
						vertices.push_back(vertex);
					}
        }
        else if(cmd == "tri"){
          validinput = readvalsi(s,3,values_i); // 
					if (validinput) {
						vec4 vertex0 = vertices[values_i[0]];
						vec4 vertex1 = vertices[values_i[1]];
						vec4 vertex2 = vertices[values_i[2]];

						shared_ptr<Triangle> t( new Triangle(vertex0, vertex1, vertex2));
						// t->diffuse = diffuse;
						// t->specular = specular;
						// t->emission = emission;
						// t->shininess = shininess;
						// t->ambient = ambient;
            t->material = Material(diffuse, ambient, specular, emission, shininess);
						t->transform = transfstack.top();
						//triangles.push_back(t);
            scene.push_back(t);
            
					}
        }
        else if(cmd == "sphere"){
          validinput = readvalsf(s,4,values);
          if(validinput){
            vec3 center = vec3(values[0], values[1], values[2]);
            float radius = values[3];
            shared_ptr<Sphere> sp(new Sphere(center, radius));
            // sp.diffuse = diffuse;
						// sp.specular = specular;
						// sp.emission = emission;
						// sp.shininess = shininess;
						// sp.ambient = ambient;
            sp->material = Material(diffuse, ambient, specular, emission, shininess);
						sp->transform = transfstack.top();
						//spheres.push_back(sp);
            scene.push_back(sp);

          }
        }
        else if (cmd == "translate") {
          validinput = readvalsf(s,3,values); 
          if (validinput) {


            // YOUR CODE FOR HW 2 HERE.  
            // Think about how the transformation stack is affected
            // You might want to use helper functions on top of file. 
            // Also keep in mind what order your matrix is
            
            rightmultiply(glm::translate(glm::mat4(1.0),glm::vec3(values[0], values[1], values[2])), transfstack);

          }
        }
        else if (cmd == "scale") {
          validinput = readvalsf(s,3,values); 
          if (validinput) {
            rightmultiply(glm::scale(glm::mat4(1.0),glm::vec3(values[0], values[1], values[2])), transfstack);

            // YOUR CODE FOR HW 2 HERE.  
            // Think about how the transformation stack is affected
            // You might want to use helper functions on top of file.  
            // Also keep in mind what order your matrix is!

          }
        }
        else if (cmd == "rotate") {
          validinput = readvalsf(s,4,values); 
          if (validinput) {

            // YOUR CODE FOR HW 2 HERE. 
            // values[0..2] are the axis, values[3] is the angle.  
            // You may want to normalize the axis (or in Transform::rotate)
            // See how the stack is affected, as above.  
            // Note that rotate returns a mat3. 
            // Also keep in mind what order your matrix is!
      
             vec3 axis = vec3(values[0], values[1], values[2]);
            // axis = glm::normalize(axis);
             float angle  = values[3];// * M_PI / 180.0;
            // mat3 r = Transform::rotate(angle, axis);
            // mat4 r4 = mat4(
            //     r[0][0], r[0][1], r[0][2], 0,
            //     r[1][0], r[1][1], r[1][2], 0,
            //     r[2][0], r[2][1], r[2][2], 0,
            //           0,       0,       0, 1
            //   );

            // rightmultiply((r4), transfstack);
            rightmultiply(glm::rotate(mat4(1.0),glm::radians(angle),axis),transfstack);
            

          }
        }

        // I include the basic push/pop code for matrix stacks
        else if (cmd == "pushTransform") {
          transfstack.push(transfstack.top()); 
        } else if (cmd == "popTransform") {
          if (transfstack.size() <= 1) {
            cerr << "Stack has no elements.  Cannot Pop\n"; 
          } else {
            transfstack.pop(); 
          }
        }

        else {
          cerr << "Unknown Command: " << cmd << " Skipping \n"; 
        }
      }
      getline (in, str); 
    }

    // Set up initial position for eye, up and amount
    // As well as booleans  

  } else {
    cerr << "Unable to Open Input Data File " << filename << "\n"; 
    throw 2; 
  }
}


Camera FileReader::getCamera(){
  return camera;
}

vector<std::shared_ptr<Primitive>> FileReader::getScene(){
  return scene;
}

Size FileReader::getImageSize(){
  return Size(image_width,image_height);
}