#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "rt3-base.h"
#include "rt3.h"
#include <list>
namespace rt3 {
// TODO: Create a virtual class Background and derive BackgroundColor,
// BackgroundSphereImage, BackgroundSkyBoxImage.
/*!
 * A background is basically a rectangle, have a color associated to each
 * corner. A background might be sampled based on a normalized coordinate in
 * \f$[0,1]^2.\f$. We generate samples with bilinear interpolation of the corner
 * colors. We consider the corners in a circular clockwise order, starting at
 * the lower-left corner.
 *
 * Alternatively, a background might be a regular image.
 */
class Background {
public:
  /// Types of color mapping scheme
  enum class mapping_t {
    screen = 0,  //!< background is assigned to the image screen
    spherical    //!< background is assigne to a sphere surrounding the scene.
  };

  mapping_t mapping_type;  //!< sampling type we use to pick a color.

  /// Ctro receiving a single color for the entire background.
  Background(mapping_t mt = mapping_t::spherical) : mapping_type{ mt } { /* empty */ }

  virtual ~Background(){ /* empty */ };
  [[nodiscard]] virtual Spectrum sampleXYZ(const Point2f& pixel_ndc) const = 0;
};

class BackgroundColor : public Background {


  /// Each corner has a color associated with.
private:
  Spectrum corners[4] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
  /// Corner indices.
  enum Corners_e {
    bl = 0,  //!< Bottom left corner.
    tl,      //!< Top left corner.
    tr,      //!< Top right corner.
    br       //!< Bottom right corner.
  };

public:
  /// Ctro receives a list of four colors, for each corner.
  BackgroundColor(const std::list<Spectrum> &colours)  //: Background(mapping_t::screen)
  {
    if (colours.size() == 1) { 
      for (int i = 0; i < 4; ++i) {
        corners[i] = colours.front();
      }
    } else if (colours.size() == 4) {
      auto it = colours.begin();
      for (int i = 0; i < 4; ++i) {      
        corners[i] = *it;
        it++;
      }
    }

    //  for (auto i=0; i < 4; i++)
    //    std::clog << i<<" --- CORES -->  [" << corners[i][0] 
    //    << " - " << corners[i][1] 
    //    << " - " << corners[i][2] 
    //    << "]\n";
  }

  friend Spectrum addColor(const Color24& );
  
  [[nodiscard]] virtual Spectrum sampleXYZ(const Point2f& pixel_ndc) const override;
  
  virtual ~BackgroundColor(){};
};

// factory pattern functions.
BackgroundColor* create_color_background(const ParamSet& ps);
}  // namespace rt3
#endif
