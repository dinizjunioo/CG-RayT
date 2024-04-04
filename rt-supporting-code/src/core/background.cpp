#include "background.h"

namespace rt3 {
/*!
 * Samples a color base on spherical interpolation of an image ou colored
 * background.
 *
 * @param pixel_ndc Pixel position in NDC space,  in \f$[0:1]\f$.
 * \return The interpolated color.
 */
Spectrum Background::sampleXYZ(const Point2f& pixel_ndc) const {
   // Interpolação linear horizontal entre os cantos inferiores (bl e br)
    //Spectrum bottom = glm::lerp(corners[bl], corners[br], pixel_ndc[0]);
  Spectrum bottom = rt3::Lerp(corners[bl], corners[br], pixel_ndc[0]);
    // Interpolação linear horizontal entre os cantos superiores (tl e tr)
  Spectrum top    = rt3::Lerp(corners[tl], corners[tr], pixel_ndc[0]);

    // Interpolação linear vertical entre as cores interpoladas horizontalmente
  return Spectrum{rt3::Lerp(bottom, top, pixel_ndc[1])};
}

BackgroundColor * create_color_background(const ParamSet& ps) {
  // Tenta recuperar os valores da cor do ParamSet
    auto color_value = rt3::retrieve<Color24>(ps, "color", {0, 255, 0}); // Obtém os valores de cor, padrão verde

    // Converte os valores de cor para RGBColor
    Spectrum color {
        static_cast<int>(color_value[0]),
        static_cast<int>(color_value[1]),
        static_cast<int>(color_value[2])
    };
  return new BackgroundColor(color);
}
}  // namespace rt3
