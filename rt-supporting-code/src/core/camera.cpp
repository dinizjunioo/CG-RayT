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
      screen_window = rt3::retrieve<std::vector<real_type>>(ps, "screen_window", {-1.555,1.555, -1,1});
    }    

    // Calculate camera frame
    Vector3f gaze = look_at - look_from;
    normalize(gaze); 
    w = gaze;
    u = normalize(cross(vup, w));  
    v = normalize(cross(w, u));    
    e = look_from;  // Camera origin
}

/// métodos da camera orthografica
Ray OrthographicCamera::generate_ray(int x, int y){ // são as coordendas
    // Calculate ray direction and origin for orthographic camera
    Vector3f ray_direction = w;
    Point3f ray_origin = e + prod_escalar(x, u) + prod_escalar(y , v); // Assuming u and v are the screen coordinates
    return Ray(ray_origin, ray_direction);
}

/// métodos da camera perspectiva
Ray PerspectiveCamera::generate_ray(int x, int y)
{
  // Calculate ray direction and origin for perspective camera
    Point3f ray_origin = e; 
    Vector3f ray_direction = u;
    //prod_escalar(x , u) + prod_escalar(y , v); // Alterado para adição
    //prod_escalar(focal_distance, w) +
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