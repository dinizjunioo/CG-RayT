#include "camera.h"

namespace rt3{


// camera

void Camera::read_info_camera(const ParamSet& ps)
{
   for (const auto& entry : ps){
      std::cout << "Key: " << entry.first << ", Value: \n";
   }
    if(ps.find("look_at") != ps.end()){
      look_at = rt3::retrieve<Point3f>(ps, "look_at", {0, 255, 0}); // Obtém os valores de cor, padrão verde
      std::cout << "look_at->" <<  " "<< look_at.x <<  " " << look_at.y << " " << look_at.z;
    }
    if (ps.find("look_from") != ps.end()) {
      look_from = rt3::retrieve<Point3f>(ps, "look_from", {0, 255, 0});
      std::cout << "look_from->" <<  " "<< look_from.x <<  " " << look_from.y << " " << look_from.z;
    } 
    if (ps.find("up") != ps.end()) {
      vup = rt3::retrieve<Vector3f>(ps, "up", {0, 255, 0});
      std::cout << "up->" <<  " "<< vup.x <<  " " << vup.y << " " << vup.z;
    } 
    if (ps.find("screen_window") != ps.end()) {
      screen_window = rt3::retrieve<arr_real_type>(ps, "screen_window", {-1.555,1.555, -1,1});
      
      // tem um jeito mais  elegante e seguro de fazer isso com enum class ... 
      auto ptr_screen = screen_window.begin();
      for (long unsigned int i = 0; i < viewPlane.size(); ++i) {      
        if (ptr_screen == screen_window.end()) {
          throw std::runtime_error("screen_window array is too short");
        }
        viewPlane[i] = *ptr_screen++;
      }

      //std::cout << "[          ]" << viewPlane[0] <<" " <<viewPlane[2] << "-------\n";

    }    
    // Calculate camera frame
    Vector3f gaze = look_at - look_from;
    glm::normalize(gaze); 
    w = gaze;
    u = glm::cross(vup, w);  
    glm::normalize(u);
    v = glm::cross(w, u);   
    glm::normalize(v); 
    e = look_from;  // Camera origin
}

/// métodos da camera orthografica
Ray OrthographicCamera::generate_ray(int x, int y){ // são as coordendas
    // Calculate ray direction and origin for orthographic camera
    
    // Calculate ray origin based on screen coordinates
    // float l = screen_window[0];
    // float r = screen_window[1];
    // float b = screen_window[2];
    // float t = screen_window[3];

    // float u_ratio = static_cast<float>(x) / static_cast<float>(2800);
    // float v_ratio = static_cast<float>(y) / static_cast<float>(1800);

    // Point3f ray_origin = e + l * u + b * v + u_ratio * (r - l) * u + v_ratio * (t - b) * v;

    Point3f ray_origin = e + (static_cast<float>(x) * u) + (static_cast<float>(y) * v); // Assuming u and v are the screen coordinates
    
    Vector3f ray_direction = w;
    return Ray(ray_origin, ray_direction);
}

/// métodos da camera perspectiva
Ray PerspectiveCamera::generate_ray(int x, int y)
{
  // Calculate ray direction and origin for perspective camera
    Point3f ray_origin = e; 
    glm::vec3 ray_direction = (focal_distance * w) + (static_cast<float>(x) * u) + (static_cast<float>(y) * v);
    return Ray(ray_origin, ray_direction);   
}

/// factory pattern functions.
///
OrthographicCamera * create_camera_orthographic(const ParamSet& ps)
{   
    return new OrthographicCamera(ps);
}
///
PerspectiveCamera * create_camera_perspective(const ParamSet& ps)
{
    return new PerspectiveCamera(ps);
}

}


// void OrthographicCamera::create_cam_orthographic(void)
// {

// }
// }
// void PerspectiveCamera::create_cam_Perspective(void){
//   //
// }