#ifndef CAMERA_H
#define CAMERA_H

#include "rt3-base.h"
#include "rt3.h"
#include "ray.h"
//#include <list>
namespace rt3{
/**
 * @brief  TEM QUE AJEITAR O CMAKELIST
 * 
 * 
 */

// { param_type_e::ARR_REAL, "vpdim" },  // The viewport dimensions defined by the scene
//         // Legacy parameters ENDS.
//         { param_type_e::REAL, "fovy" },
//         { param_type_e::REAL, "focal_distance" },
//         { param_type_e::REAL, "frame_aspectratio" },  // By default it's calulated from the x/y
//                                                       // resolution, but it might be overridden.
//         { param_type_e::ARR_REAL, "screen_window" }   // Bounds of the film in screen space. [-1,1]
//                                                       // along the shorter image axis, and its set
//                                                       // proportionally along the longer axis.
//       };
class Camera{
public:
	/// types of cameras
	enum class camera_t{
		orthographic = 0, 	// camera com projeção paralela (mesma direcao, diferentes origens) - fovy = 0
		perspective			// camera com projecao (diferentes direcoes, mesma origem) - fovy > 0
	};
	camera_t camera_type;
	///
	//const Point3f& look_from, const Point3f& look_at, const Vector3f& vup, const arr_real_type& screen_window,	
	//look_at(look_at), look_from(look_from), vup(vup), screen_window(screen_window),
	Camera(camera_t ct, const ParamSet& ps)
	 :  camera_type{ct} { 
		std::cout << "criando minha camera\n"; 
		focal_distance = 1; 
		read_info_camera(ps);
		}
	//
	virtual Ray generate_ray(int u, int v) = 0;
	//
	virtual ~Camera() = default;
public:

/**
 * @brief 
 * @param e --> olho da camera
 * @param look_from --> 
 * @param look_at -->   
 * @param vup --> vetor up 
 * @param w --> vetor que tem direcao ao plano da imagem 
 * @param u
 * @param v	--> 
 * @param focal_distance --> distancia do olho da camera até a "image plane" ao longo do eixo Z
 * @param screen_window
 */

	Vector3f look_at;
	Vector3f look_from;
	Vector3f vup;
	Point3f e;
	Vector3f u,v,w;
	float focal_distance;
	arr_real_type screen_window;
	std::array<real_type,4> viewPlane = {0,0,0,0};

private:
	void read_info_camera(const ParamSet& ps);

  enum ViewPlaneDimensions_e {
    left = 0,  //!< 
    right,      //!< 
    bottom,      //!<
    top       //!< 
  }; 
};

class PerspectiveCamera : public Camera {
public:
	PerspectiveCamera(const ParamSet& ps) : Camera(camera_t::perspective, ps) 
	{

		std::cout << "criando minha camera perspectiva\n";
		//create_cam_Perspective();
	}

	Ray generate_ray(int u, int v) override;
	virtual ~PerspectiveCamera(){ /* --- */}
private:
	//Vector3f gaze;
	int fovy;
public:
	//
private:
	//void create_cam_Perspective(void);
};

class OrthographicCamera : public Camera {
public:
	//
	OrthographicCamera(const ParamSet& ps) : Camera(camera_t::orthographic, ps)
	{
		std::cout << "criando minha camera ortografica\n";
		// inializar e fazer o que tem que fazer aqui
		//create_cam_orthographic();
	}
	//
	Ray generate_ray(int u, int v) override;			
	virtual ~OrthographicCamera() = default;
private:
	//
	//void create_cam_orthographic(void);
public:
	// factory pattern functions.
	// friend OrthographicCamera * create_camera_orthographic(const ParamSet& ps);
};
OrthographicCamera * create_camera_orthographic(const ParamSet& ps);
PerspectiveCamera  * create_camera_perspective(const ParamSet& ps);
}

#endif