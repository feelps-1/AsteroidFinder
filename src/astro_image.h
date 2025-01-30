#include <opencv2/opencv.hpp>
#include <CCfits/CCfits>
#include <iostream>
#include <valarray>
#include <vector>

using namespace std;
using namespace cv;
using namespace CCfits;

class AstroImagem{
private:
    Mat image;
    long altura, largura;
    string timestamp;
    string imgPath;
    Rect overscanROI;

    Mat limparComOverscan(Mat&, const Rect&);
    Mat detectarRuidoEletronico(const Mat&);
    Rect parseBIASSEC(const string&);

public:
    AstroImagem(const string&);
    AstroImagem(const AstroImagem&);

    void setImage(const Mat& matriz) { image = matriz; }
    cv::Mat getImage() { return image; }

    long getLargura() { return largura; }
    long getAltura() { return altura; }

};
