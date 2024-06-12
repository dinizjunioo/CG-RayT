#include <cstdint>
#ifndef RT3_H
# define RT3_H 1

# include <cassert>
# include <cmath>
# include <cstdlib>
# include <iostream>
# include <memory>
# include <vector>
using std::vector;
# include <array>
using std::array;
# include <map>
# include <stack>
# include <string>
# include <unordered_map>
using std::string;
# include <algorithm>
# include <tuple>
using std::tuple;
# include <utility>

// usaremos a biblioteca matematica glm 
#include <../glm/glm.hpp>
//== Alias to the chosen data structure to implement a dictionary.
// #define Dictionary std::unordered_map
# define Dictionary std::map

//=== Aliases
namespace rt3 {

/// ATENTION: This is just to compile the project. You need to implement
/// Point3f!!!!
// Alias to a Point3f (simulation)
// Temporary Vec3, just to compile. TODO: Implement a real one!!!
using Point3f = glm::vec3;
using Vector3f = glm::vec3;
using Color24 = std::array<uint8_t, 3UL>;
//using Color24 = std::vector<glm::tvec3<uint8_t>>;
using Spectrum = std::array<float, 3>;
using Normal3f = std::array<float, 3>;
//using Ray = std::array<float, 3>;

// List of points
using ListPoint3f = std::vector<Point3f>;

using Vector3i = glm::vec3;
using Point3i = std::array<int, 3>;
using Point2i = std::array<int, 2>;
using Point2f = std::array<float, 2>;

template <typename T, size_t S>
std::ostream& operator<<(std::ostream& os, const std::array<T, S>& v) {
  os << "[ ";
  for (const auto& e : v) {
    os << e << " ";
  }
  os << "]";
  return os;
}

// Global Forward Declarations
class Film;
class Background;
class BackgroundColor;
class Camera;
class Ray;
// class LookAt;

//=== aliases
using real_type = float;
using size_type = size_t;
using result_type = std::tuple<bool, std::string>;
using arr_real_type = std::vector<real_type>;
/// This struct holds information provided via command line arguments
struct RunningOptions {
  // An alias template for a two-dimensional std::array
  template <typename T, std::size_t Row, std::size_t Col> using Array2d
    = std::array<std::array<T, Col>, Row>;

  // Vanilla ctro.
  RunningOptions() = default;

  // 1 = 100% of the full resolution.
  // [row=0] -> X; [row=1] -> Y
  // x0, x1, y0, y1
  Array2d<real_type, 2, 2> crop_window{ { { 0, 1 }, { 0, 1 } } };  //!< Crop window to render.
  std::string filename;                                            //!< input scene file name.
  std::string outfile;                                             //!< output image file name.
  bool quick_render{ false };  //!< if true render image with 1/4 of the
                               //!< requested resolition.
};

//=== Global Inline Functions

/*! Linear interpolation.
 * \param t The parameter, in [0,1].
 * \param v1 The initial interpolation value.
 * \param v2 The final interpolation value.
 * \return The interpolated value.
 */
//

inline float Lerp(float v1, float v2, float t) { return (1.F - t) * v1 + t * v2; }

template<typename T>
T lerp(const T& A, const T& B, float t) {
    // Aplica a interpolação linear nos componentes de A e B
    T result;
    for (size_t i = 0; i < A.size(); ++i) {
        result[i] = static_cast<typename T::value_type>((1 - t) * A[i] + t * B[i]);
    }
    return result;
}

template<typename T>
T sphericalMap(const T& A, const T& B, float u, float v) {
    // Converte u e v em coordenadas esféricas
    float phi = u * 2 * M_PI;
    float theta = v * M_PI;

    // Converte coordenadas esféricas em coordenadas cartesianas
    float x = sin(theta) * cos(phi);
    float y = cos(theta);
    float z = sin(theta) * sin(phi);

    // Mapeia coordenadas cartesianas para o intervalo [0, 1]
    float mappedU = (x + 1) / 2; // Mapeamento para o intervalo [0, 1]
    float mappedV = (y + 1) / 2; // Mapeamento para o intervalo [0, 1]

    // Aplica a interpolação linear nos componentes de A e B
    T result;
    for (size_t i = 0; i < A.size(); ++i) {
        result[i] = static_cast<typename T::value_type>((1 - mappedU) * A[i] + mappedU * B[i]);
    }
    return result;
}


/// Clamp T to [low,high].
template <typename T, typename U, typename V> inline T Clamp(T val, U low, V high) {
  if (val < low) {
    return low;
  }
  if (val > high) {
    return high;
  }
  return val;
}

/// Degrees to radians.
inline float Radians(float deg) { return ((float)M_PI / 180.F) * deg; }

/// Radians to degreees.
inline float Degrees(float rad) { return (180.F / (float)M_PI) * rad; }

// template<typename T>
// inline T normalize (const T& v, const T& u ){ return glm::cross(v,u);}
// template<typename T>
// inline void normalize (T& w){ w = glm::cross(w);}

// template<typename T, typename U>
// auto prod_escalar(const T& scalar, const U& vector) {
//     using ResultType = decltype(scalar * vector.x);
//     return ResultType(scalar * vector.x, scalar * vector.y, scalar * vector.z);
// }

}  // namespace rt3

// Spectrum LerSpectrum(const Spectrum &A, const Spectrum &B, float t) {
//     // Aplica a interpolação linear nos componentes RGB de A e B
//     return Spectrum {
//     (1 - t) * A[0] + t * B[0],
//     (1 - t) * A[1] + t * B[1],
//     (1 - t) * A[2] + t * B[2]
//     };
// }

#endif  // RT3_H
