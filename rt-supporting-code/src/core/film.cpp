#include "film.h"
#include <cmath>

#include "api.h"
#include "image_io.h"
#include "paramset.h"

namespace rt3 {

//=== Film Method Definitions
Film::Film(const Point2i& resolution, const std::string& filename, image_type_e imgt, bool gamma)
    : m_full_resolution{ resolution }, m_filename{ filename }, m_image_type{ imgt },
    m_gamma_corrected{gamma}, image_data{ Matriz(resolution[1], resolution[0], 3)} {
    
    //Matriz * data{ nullptr };
    //this->image_data{create_matriz(resolution)};
}

Film::~Film() = default;

/// Add the color to image.
void Film::add_sample(const Point2f& pixel_coord, const Color24& pixel_color) {
  // add color to the proper location.
  // image_data.getData()[] = pixel_color;
  //std::cout << "[" << static_cast<int>(pixel_coord[0]) << " " << static_cast<int>(pixel_coord[1]);
  //std::cout << " " << static_cast<int>(pixel_color[0])<< " " <<  static_cast<int>(pixel_color[1]) << " " << static_cast<int>(pixel_color[2]) << "\n";
  image_data.setColor(static_cast<int>(pixel_coord[0]),static_cast<int>(pixel_coord[1]), 3, pixel_color);
}

void Film::corrected_gamma(int h, int w, MtxColor& ptr_color)const
{
  std::cout << "entrei no gamma " << "\n";
  // Aplicar a correção de gama à imagem antes de salvá-la
  constexpr double gamma = 2.2; // Valor de correção de ga
  for (auto j = 0; j < h; j++) {
      for (auto i = 0; i < w; i++) {
          // Obter o pixel atual
          Color24& pixel = ptr_color[j * w + i];
          // Aplicar a correção de gama a cada componente de cor do pixel
          for (int c = 0; c < 3; c++) {
              pixel[c] = std::pow(pixel[c] / 255.0, 1.0 / gamma) * 255.0;
          }
      }
  }
}


/// Convert image to RGB, compute final pixel values, write image.
void Film::write_image() const {
  
  //image_data.lerMatriz();
  auto w = m_full_resolution[0];
  auto h = m_full_resolution[1];
  auto d = 3; // Assume que a profundidade da imagem é 3 (RGB)

  auto ptr_color = image_data.getData();
  
  if (m_gamma_corrected) {
    corrected_gamma(h, w, ptr_color);     
  }

  uint8_t * ptr = &ptr_color[0][0];

  switch (m_image_type) 
  {
    case image_type_e::PNG:
      std::clog << "foto em png sendo criada...\n";
      //reinterpret_cast<unsigned char*>
      save_png(reinterpret_cast<unsigned char*>(ptr), w, h, d, m_filename);
      break;
    case image_type_e::PPM3:
     std::clog << "foto em ppm3...\n";
      save_ppm3(reinterpret_cast<unsigned char*>(ptr), w, h, d, m_filename);
      break;
    case image_type_e::PPM6:
      save_ppm6(reinterpret_cast<unsigned char*>(ptr), w, h, d, m_filename);
      break;
    default:
      RT3_ERROR("Invalid image type specified");
      break;
  }

}

// Factory function pattern.
// This is the function that retrieves from the ParamSet object
// all the information we need to create a Film object.
Film* create_film(const ParamSet& ps) {
  std::cout << ">>> Inside create_film()\n";
  std::string filename;
  // Let us check whether user has provided an output file name via
  // command line arguments in main().
  if (not API::curr_run_opt.outfile.empty()) {
    // Retrieve filename defined via CLI.
    filename = API::curr_run_opt.outfile;
    // Retrieve another filename, now from the ParamSet object.
    std::string filename_from_file = retrieve(ps, "filename", std::string{ "" });
    if (not filename_from_file.empty()) {  // We also get a filename from scene file...
      RT3_WARNING(string{ "Output filename supplied on command line, \"" }
                  + API::curr_run_opt.outfile
                  + string{ "\" is overriding filename provided in scene "
                            "description file, \"" }
                  + filename_from_file);
    }
  } else {
    // Try yo retrieve filename from scene file.
    filename = retrieve(ps, "filename", std::string{ "image.png" });
  }

  // Read resolution.
  // Aux function that retrieves info from the ParamSet.
  int xres = retrieve(ps, "x_res", int(1280));
  // Aux function that retrieves info from the ParamSet.
  int yres = retrieve(ps, "y_res", int(720));
  // Quick render?
  if (API::curr_run_opt.quick_render) {
    // decrease resolution.
    xres = std::max(1, xres / 4);
    yres = std::max(1, yres / 4);
  }
  
  bool gamma = retrieve<std::string>(ps, "gamma_corrected") == "yes" ? true : false;
  std::cout << "gamma -->" << gamma << "\n";
  std::string image_type = retrieve<string>(ps, "img_type", std::string{ "png"});
  std::transform(image_type.begin(), image_type.end(), image_type.begin(), ::tolower);  
  Film::image_type_e ite = Film::image_type_e::PNG;
  if (image_type.compare("ppm3") == 0) {ite = Film::image_type_e::PPM3;}
  else if (image_type.compare("ppm6") == 0) {ite = Film::image_type_e::PPM6;}

  // Read crop window information.
  std::vector<real_type> cw = retrieve(ps, "crop_window", std::vector<real_type>{ 0, 1, 0, 1 });
  std::cout << "Crop window ";
  for (const auto& e : cw) {
    std::cout << e << " ";
  }
  std::cout << '\n';

  // Note that the image type is fixed here. Must be read from ParamSet, though.
  return new Film(Point2i{ xres, yres }, filename, ite, gamma);
}
}  // namespace rt3
