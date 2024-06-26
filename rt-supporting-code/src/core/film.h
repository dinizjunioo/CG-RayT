#ifndef FILM_H
#define FILM_H

#include "error.h"
#include "paramset.h"
#include "rt3.h"
#include <array>
#include <vector>

namespace rt3 {

//using MtxColor =  std::vector<std::vector<Color24>>;
using MtxColor =  std::vector<Color24>;

/// Represents an image generated by the ray tracer.
class Matriz{
  
private:

  int rows;
  int cols;
  int dimension;

  MtxColor data;

public:

    Matriz(int rows, int cols, int dimension) : rows(rows), cols(cols), dimension(dimension),
    //data(rows, std::vector<Color24>(cols)) 
    data(rows * cols , {255, 255, 0}) 
    { /* empty */ 
      
    }


    void setColor(int x, int y, int z,  const Color24& value) {
      try{
        if (x < 0 || x >= cols || y < 0 || y >= rows) {
                std::cerr << " -- " << cols << "--" << rows << " -- " ;
                throw std::out_of_range("Índices fora dos limites da matriz");
            }
        data.at((y * cols + x)) = value;
      } catch (const std::out_of_range& e) {
        std::cerr << "Erro ao acessar memória: " << e.what() << std::endl;
      }
      //std::clog << data.size() << " ";
    }

    MtxColor getData() const {
      return data;
    }

    // Color24& matriz_data(Point2f points)
    // {
    //   return data[static_cast<int>(points[0])][static_cast<int>(points[1])];
    // }
    
    void lerMatriz() const {
      auto mtxData = getData(); // Obtém os dados da matriz
      auto ptr = mtxData.data(); // Obtém o ponteiro para os dados

      for (auto i = 0; i < rows; i++) {
        for (auto j = 0; j < cols; j++) {
            std::cout << "Matriz[" << i << "][" << j << "]: ";
            int index = i * cols + j;
            for (int k = 0; k < 3; ++k) {
                // Acessa os dados usando o ponteiro
                std::cout << static_cast<int>(ptr[index][k]) << " ";
            }
            std::cout << std::endl;
        }
      }
    }
};


class Film {
public:
  /// List of support image file formats.
  enum class image_type_e : int { PNG = 0, PPM3, PPM6 };

  //=== Film Public Methods
  Film(const Point2i& resolution, const std::string& filename, image_type_e imgt, bool gamma);
  virtual ~Film();

  /// Retrieve original Film resolution.
  [[nodiscard]] Point2i get_resolution() const { return m_full_resolution; };
  /// Takes a sample `p` and its radiance `L` and updates the image.
  void add_sample(const Point2f&, const Color24&);
  void write_image() const;

  // functions aux
  void corrected_gamma(int h, int w, MtxColor&) const;
  
  //=== Film Public Data
  const Point2i m_full_resolution;  //!< The image's full resolution values.
  std::string m_filename;           //!< Full path file name + extension.
  image_type_e m_image_type;        //!< Image type, PNG, PPM3, PPM6.
  bool m_gamma_corrected;
  // TODO: Create the matrix (or vector) that will hold the image data.
private:
  mutable Matriz image_data;
  
  // std::unique_ptr< ColorBuffer > m_color_buffer_ptr; //!< Reference to the
  // color buffer (image) object.
};

// Factory pattern. It's not part of this class.
Film* create_film(const ParamSet& ps);
}  // namespace rt3

#endif  // FILM_H
