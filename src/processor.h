#include <opencv2/opencv.hpp>
#include <CCfits/CCfits>
#include <iostream>
#include <valarray>
#include <cmath>
#include <dirent.h>
#include <vector>
#include <algorithm>
#include "astro_image.cpp"

const double LSD_SCALE = 0.5;       // Escala da image
const double LSD_SIGMA = 0.1;       // Força do Gaussian blur
const double LSD_ANG_TH = 22.5;     // Limiar dos ângulos
const double LSD_LOG_EPS = 0.05;     // Limiar de detecção das linhas
const int LINE_MASK_WIDTH = 5;      // Grossura da linha na mascára

class AstroImagemProcessor {
private:
    bool DEBUG{false};

    vector<Mat> cleanedImages;

    Mat processarImagem(const string&);
    Mat detectarLinhas(const Mat&);   

public:
    void processarPasta(const string&);
    Mat mesclarImagens();
    void salvarPreview(const string&);
    vector<Mat> getCleanedImages() { return cleanedImages; }
    Mat encontrarRastros(const vector<Mat>&, const Mat&);
    
    void toggleDebug() { DEBUG = !DEBUG; 
                         cout << "Modo de debug ativado!" << '\n'; }
 
};
