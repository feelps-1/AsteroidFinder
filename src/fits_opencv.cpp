#include <opencv2/opencv.hpp>
#include <CCfits/CCfits>
#include <iostream>
#include <valarray>
#include <cmath>  // Para funções matemáticas

using namespace CCfits;
using namespace std;

void processarImagem(const string& inputPath, const string& outputPath) {
    try {
        // Abrindo o arquivo FITS
        std::auto_ptr<FITS> pInfile(new FITS(inputPath, Read, true));

        // Acessando o HDU principal (Primary Header Data Unit)
        PHDU& image = pInfile->pHDU();

        // Obtendo as dimensões da imagem
        long naxis = image.axis(0);
        long naxis1 = image.axis(1);
        std::cout << "Processando imagem: " << inputPath << std::endl;

        // Lendo os dados da imagem como um valarray
        std::valarray<float> imageData;
        image.read(imageData);

        // Obtendo bscale e bzero do cabeçalho
        float bscale = 1.0f, bzero = 32768.0f; // Valores padrão
        try {
            image.readKey("BSCALE", bscale);
            image.readKey("BZERO", bzero);
            std::cout << "BSCALE: " << bscale << ", BZERO: " << bzero << std::endl;
        } catch (...) {
            std::cerr << "Chaves BSCALE ou BZERO não encontradas. Usando valores padrão." << std::endl;
        }

        // Ajustando os valores dos dados
        for (size_t i = 0; i < imageData.size(); ++i) {
            imageData[i] = (imageData[i] * bscale) + bzero;
        }

        // Convertendo para uma matriz OpenCV (cv::Mat)
        cv::Mat matImage(naxis, naxis1, CV_32F);
        for (long i = 0; i < naxis1; ++i) {
            for (long j = 0; j < naxis; ++j) {
                matImage.at<float>(i, j) = imageData[i * naxis + j];
            }
        }

        // Normalização por raiz quadrada
        double minVal, maxVal;
        cv::minMaxLoc(matImage, &minVal, &maxVal);
        cv::Mat normalizedImage = matImage.clone();
        cv::sqrt(normalizedImage, normalizedImage);  // Aplicar a raiz quadrada
        normalizedImage /= std::sqrt(maxVal);        // Normalizar para o intervalo [0, 1]
        normalizedImage *= 255;                      // Escalar para [0, 255]
        normalizedImage.convertTo(normalizedImage, CV_8U);

        // Salvando a imagem resultante
        string outputFile = outputPath + "/imagem_processada.jpg";
        cv::imwrite(outputFile, normalizedImage);

        std::cout << "Imagem processada salva em: " << outputFile << std::endl;
    }
    catch (FITS::CantOpen& e) {
        std::cerr << "Erro ao abrir o arquivo FITS: " << inputPath << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
    }
}

int main() {
    string inputFile = "../Survey_2019_292_Cleaned_Orbit_4/NEOS_SCI_2019292041030_cord.fits";  // Arquivo de entrada
    string outputDir = "../PROCESSADAS";         // Diretório de saída

    // Processar a imagem
    processarImagem(inputFile, outputDir);

    std::cout << "Processamento concluído!" << std::endl;
    return 0;
}
