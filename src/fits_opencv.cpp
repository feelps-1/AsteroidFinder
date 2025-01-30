#include <opencv2/opencv.hpp>
#include <CCfits/CCfits>
#include <iostream>
#include <valarray>
#include <cmath>
#include <dirent.h>
#include <vector>
#include <algorithm>
#include "processor.cpp"

using namespace std;
using namespace CCfits;
using namespace cv;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <pasta de entrada> <pasta de saída>" << endl;
        return 1;
    }

    string inputFolder = argv[1];
    string outputFolder = argv[2];
    string mergepath = outputFolder + "/merged_preview.png";

    AstroImagemProcessor processor;

    // Process all images in the provided input folder
    processor.processarPasta(inputFolder);

    // Save the merged preview image
    processor.salvarPreview(outputFolder);

    // Uncomment to enable debugging and visualization
    //processor.toggleDebug();

    vector<Mat> individualImages = processor.getCleanedImages();

    Mat tracks = processor.encontrarRastros(individualImages, imread(mergepath));

    Mat merged_color = imread(mergepath);
    if (merged_color.channels() == 1) {
        cvtColor(merged_color, merged_color, COLOR_GRAY2BGR);
    }
    merged_color.setTo(Scalar(0, 255, 0), tracks);

    // Save the final processed result
    string resultPath = outputFolder + "/result.jpg";
    imwrite(resultPath, merged_color);

    cout << "Dataset processado com sucesso! Resultado em: " << resultPath << '\n';

    return 0;
}
