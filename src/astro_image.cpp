#include <opencv2/opencv.hpp>
#include <CCfits/CCfits>
#include <iostream>
#include <valarray>
#include <cmath>
#include <dirent.h>
#include <vector>
#include <algorithm>
#include "astro_image.h"

using namespace std;
using namespace CCfits;
using namespace cv;

AstroImagem::AstroImagem(const string& filePath) : imgPath(filePath){
    unique_ptr<FITS> pInFile(new FITS(filePath, Read, true));
        PHDU& imgHeader = pInFile->pHDU();

        // Lê as dimensões da imagem
        altura = imgHeader.axis(1);
        largura = imgHeader.axis(0);
        string biassec;
        imgHeader.readKey("BIASSEC", biassec);

        // Pega as coordenadas do overscan [x1:x2,y1:y2]
        overscanROI = parseBIASSEC(biassec);
        
        // Salva os dados da imagem
        valarray<float> imgData;
        imgHeader.read(imgData);

        // Aplica BSCALE\BZERO
        float bscale = 1.0, bzero = 0.0;
        try {
            imgHeader.readKey("BSCALE", bscale);
            imgHeader.readKey("BZERO", bzero);
        } catch(...) {
            cerr << "Usando padrão" << endl;
        }
        imgData = (imgData * bscale) + bzero;

        // Usa os dados para transformar numa matriz do OpenCV
        Mat imagemOriginal(altura, largura, CV_32F);
        for(long linha=0; linha<altura; ++linha) {
            for(long col=0; col<largura; ++col) {
                imagemOriginal.at<float>(linha, col) = imgData[linha*largura + col];
            }
        }

        // Usa a técnica de overscan do artigo
        Mat imagemLimpa = limparComOverscan(imagemOriginal, overscanROI);

        setImage(imagemLimpa);

        // Armazena o timestamp da imagem
        string t;
        imgHeader.readKey("DATE-OBS", t);
        timestamp = t;
}

//Copy constructor
AstroImagem::AstroImagem(const AstroImagem& imagem){
    image = imagem.image;
    altura, largura = imagem.altura, imagem.largura;
    timestamp = imagem.timestamp;
    imgPath = imagem.imgPath;
    overscanROI = imagem.overscanROI;
}

cv::Rect AstroImagem::parseBIASSEC(const string& biassec){
    Rect overscanROI;
    int x1, x2, y1, y2;
    if(sscanf(biassec.c_str(), "[%d:%d,%d:%d]", &x1, &x2, &y1, &y2) == 4) {
        overscanROI = Rect(x1-1, y1-1, x2-x1+1, y2-y1+1); 
    }

    return overscanROI;
}

cv::Mat AstroImagem::limparComOverscan(Mat& imagemOriginal, const Rect& overscanROI){
    //1. Verificar se a imagem tem uma região de Overscan
    if(overscanROI.width <= 0 || overscanROI.height <= 0 ||
       overscanROI.x + overscanROI.width > imagemOriginal.cols ||
        overscanROI.y + overscanROI.height > imagemOriginal.rows) {
            cerr << "Região BIASSEC inválida, correção com Overscan ignorada" << "\n";
            return imagemOriginal.clone();
        };

    //2. Extrair a região de Overscan da imagem
    Mat overscan = imagemOriginal(overscanROI).clone();

    //3. Calcular a média da região de Overscan
    Scalar mediaOverscan = mean(overscan);
    Mat imagemLimpa = imagemOriginal - mediaOverscan[0];

    //4. Procurar por ruído eletrônico
    Mat padraoDoRuido = detectarRuidoEletronico(overscan);

    //5. Transformar o ruído para o tamanho da imagem
    Mat padraoDoRuidoCompleto;
    cv::resize(padraoDoRuido, padraoDoRuidoCompleto, imagemOriginal.size(), 0, 0, INTER_LINEAR);

    //6. Subtrair o ruído da imagem
    imagemLimpa -= padraoDoRuidoCompleto;

    return imagemLimpa;
};

cv::Mat AstroImagem::detectarRuidoEletronico(const Mat& overscan){
    //1. Calcular FFT na imagem
    Mat padded;
    int optRows = getOptimalDFTSize(overscan.rows);
    int optCols = getOptimalDFTSize(overscan.cols);
    copyMakeBorder(overscan, padded, 0, optRows - overscan.rows,
                0, optCols - overscan.cols, BORDER_CONSTANT, Scalar::all(0));

    //2. Calcular DFT
    Mat planos[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexOverscan;
    merge(planos, 2, complexOverscan); 
    dft(complexOverscan, complexOverscan);  

    //3. Separa os componentes reais e imaginários
    split(complexOverscan, planos);

    //4. Calcula a magnitude
    Mat magnitude;
    cv::magnitude(planos[0], planos[1], magnitude);  

    //5. Cria uma máscara de ruído
    double maxVal;
    minMaxLoc(magnitude, nullptr, &maxVal);
    Mat noiseMask = magnitude > (1 * maxVal);

    //6. Reconstroi o padrão do ruído
    Mat noiseComplex;
    merge(planos, 2, noiseComplex);
    noiseComplex.setTo(Scalar::all(0), ~noiseMask);
    idft(noiseComplex, noiseComplex, DFT_SCALE | DFT_REAL_OUTPUT);

    //7. Extrai o padrão do ruído
    Mat noisePattern;
    noiseComplex(Rect(0, 0, overscan.cols, overscan.rows)).copyTo(noisePattern);

    //8. Normaliza o ruído
    noisePattern -= mean(noisePattern)[0];  
    normalize(noisePattern, noisePattern, -1, 1, NORM_MINMAX);

    return noisePattern;
};
