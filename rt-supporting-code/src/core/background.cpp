#include "background.h"

namespace rt3 {
/*!
 * Samples a color base on spherical interpolation of an image ou colored
 * background.
 *
 * @param pixel_ndc Pixel position in NDC space,  in \f$[0:1]\f$.
 * \return The interpolated color.
 */
Spectrum BackgroundColor::sampleXYZ(const Point2f& pixel_ndc) const 
{
  //
  // Interpolação linear horizontal entre os cantos inferiores (bl e br)
  Spectrum bottom = rt3::LerpSpectrum(corners[bl], corners[br], pixel_ndc[0]);
  // Interpolação linear horizontal entre os cantos superiores (tl e tr)
  Spectrum top    = rt3::LerpSpectrum(corners[tl], corners[tr], pixel_ndc[0]);

  // Interpolação linear vertical entre as cores interpoladas horizontalmente
  return Spectrum{rt3::LerpSpectrum(bottom, top, pixel_ndc[1])};
}

BackgroundColor* create_color_background(const ParamSet& ps) {

  if (ps.find("color") != ps.end() && (ps.find("bl") != ps.end() || ps.find("tl") != ps.end() || ps.find("tr") != ps.end() || ps.find("br") != ps.end())) {
      throw std::runtime_error("Ambos 'color' e 'bl/tl/tr/br' encontrados. Deve ser fornecido apenas um.");
  } else if (ps.find("color") != ps.end()) {
      // Processar como uma única cor
      std::shared_ptr<ValueBase> colorValue = ps["color"];
      if (colorValue) {
          // Acesse a cor aqui
      }
  } else if (ps.find("bl") != ps.end() &&
             ps.find("tl") != ps.end() &&
             ps.find("tr") != ps.end() &&
             ps.find("br") != ps.end()) {
      // Processar como cores dos cantos
      std::shared_ptr<ValueBase> blValue = ps["bl"];
      if (blValue) {
          // Acesse a cor do canto inferior esquerdo aqui
      }
      // Repita para os outros cantos
  } else {
      throw std::runtime_error("Nenhuma informação de cor válida fornecida.");
  }


  // Tenta recuperar os valores da cor do ParamSet
  auto color_value = rt3::retrieve<Color24>(ps, "color", {0, 255, 0}); // Obtém os valores de cor, padrão verde
  if(color_value != 0){

  }

  corners[4]
  { param_type_e::COLOR, "color" },      // Single color for the entire background.
        { param_type_e::COLOR, "tl" },         // Top-left corner
        { param_type_e::COLOR, "tr" },         // Top-right corner
        { param_type_e::COLOR, "bl" },         // Bottom-left corner
        { param_type_e::COLOR, "br" }          // Bottom-right corner


    // Converte os valores de cor para RGBColor
    Spectrum color {
        static_cast<int>(color_value[0]),
        static_cast<int>(color_value[1]),
        static_cast<int>(color_value[2])
    };
  return new BackgroundColor();
}
}  // namespace rt3
