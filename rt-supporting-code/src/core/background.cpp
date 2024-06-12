#include "background.h"

namespace rt3 {
/*!
 * Samples a color base on spherical interpolation of an image ou colored
 * background.
 *
 * @param pixel_ndc Pixel position in NDC space,  in \f$[0:1]\f$.
 * \return The interpolated color.
 */

// 
// const Point2f& pixel_ndc

/**
 * @brief 
 * 
 * @param pixel_ndc
 * como o tipo pode ser  const Ray& ray ou um const Point2f& pixel_ndc
 * valor criar um generico T e chamar de pixel 
 * @return Spectrum 
 */

Spectrum BackgroundColor::sampleXYZ(const Point2f& pixel_ndc) const 
{
  // if (mapping_type == mapping_t::screen)
  // {
  // }else if (mapping_type == mapping_t::spherical){
  //   
  // }
  // Interpolação linear horizontal entre os cantos inferiores (bl e br)
  Spectrum bottom = rt3::lerp(corners[bl], corners[br], pixel_ndc[0]);
  // Interpolação linear horizontal entre os cantos superiores (tl e tr)
  Spectrum top    = rt3::lerp(corners[tl], corners[tr], pixel_ndc[0]);
  // Interpolação linear vertical entre as cores interpoladas horizontalmente
  return Spectrum{rt3::lerp(bottom, top, pixel_ndc[1])};
}


Spectrum BackgroundColor::sampleXYZ(const Ray& ray)  const
{
  // Vector3f dir = ray.direction();
  auto t = 0.5f * (ray.direction().y + 1.0f);
  // Interpolação linear entre as cores branco e azul
  return lerp(Spectrum{255, 255, 255}, Spectrum{128, 179, 255}, t);
 

  // // Normalize the direction vector
  //   glm::vec3 dir = glm::normalize(ray.direction);

  //   // Sample the spherical background
  //   return sphericalMap(dir);
}

Spectrum addColor(const Color24& colour)
{
  //colour[0] < 1.0f ? colour[0] * 255.0f : colour[0];
  return {
        static_cast<float>(colour[0]),
        static_cast<float>(colour[1]),
        static_cast<float>(colour[2])
    };
}

BackgroundColor * create_color_background(const ParamSet& ps) 
{

  // Processar como uma única cor
  std::list<Spectrum> colours;

  if(ps.find("color") != ps.end()){
    Color24 color_value = rt3::retrieve<Color24>(ps, "color", {0, 255, 0}); // Obtém os valores de cor, padrão verde
    ///std::clog << "[" << color_value[0] << " - " << color_value[1] << " - " << color_value[2] << "]\n";
    // Converte os valores de cor para RGBColor
    colours.push_back(addColor(color_value));

  } else {
    Color24 color_value = {0, 255, 0};
    if(ps.find("bl") != ps.end()){
      color_value = rt3::retrieve<Color24>(ps, "bl", {0, 255, 0}); // Obtém os valores de cor, padrão verde
      colours.push_back(addColor(color_value));
    }
    if (ps.find("tr") != ps.end()) {
      color_value = rt3::retrieve<Color24>(ps, "tr", {0, 255, 0});
      colours.push_back(addColor(color_value));
    } 
    if (ps.find("tl") != ps.end()) {
      color_value = rt3::retrieve<Color24>(ps, "tl", {0, 255, 0});
      colours.push_back(addColor(color_value));
    } 
    if (ps.find("br") != ps.end()) {
      color_value = rt3::retrieve<Color24>(ps, "br", {0, 255, 0});
      colours.push_back(addColor(color_value));
    }
  }
    return new BackgroundColor(colours);
    // não precisa de -> return std::make_unique<BackgroundColor>(colours);
    // retornando um ponteiro cru para o std unique e ele gerencia e destroi automaticamente
}
}  // namespace rt3
