#include "processor.h"

Mat AstroImagemProcessor::processarImagem(const string& i){
    //Limpa a imagem
    AstroImagem FITS_Image(i);
    Mat rawImage = FITS_Image.getImage();
        
    Mat denoised;
    bilateralFilter(rawImage, denoised, 
                5,      
                50.0,   
                50.0);  
    
    Mat sorted;
    cv::sort(denoised.reshape(1, 1), sorted, SORT_ASCENDING);
    double clipPercent = 3.0; 
    double minVal = sorted.at<float>(0, sorted.cols * clipPercent/100.0);
    double maxVal = sorted.at<float>(0, sorted.cols * (1.0 - clipPercent/100.0));

    Mat clipped = denoised;
    clipped.setTo(minVal, clipped < minVal);
    clipped.setTo(maxVal, clipped > maxVal);

    Mat normalized;
    normalize(clipped, normalized, 0, 255, NORM_MINMAX, CV_8U);

    Mat medianBlurred;
    medianBlur(normalized, medianBlurred, 51);

    Mat subtracted = normalized - medianBlurred;

    Mat NLdenoised;
    fastNlMeansDenoising(subtracted, NLdenoised, 10, 7, 21);

    Ptr<CLAHE> clahe = createCLAHE();
    clahe->setClipLimit(2.0); 
    Mat claheEnhanced;
    clahe->apply(NLdenoised, claheEnhanced);
    
    return claheEnhanced;
}

Mat AstroImagemProcessor::mesclarImagens(){
    if (cleanedImages.empty()) {
            cerr << "Sem imagens disponíveis" << endl;
            return Mat();
        }

        // Mescla todas as imagens já limpas pegando os valores máximos
        Mat merged = cleanedImages[0].clone();
        for (size_t i = 1; i < cleanedImages.size(); ++i) {
            max(merged, cleanedImages[i], merged);
        }

        return merged;
}

void AstroImagemProcessor::processarPasta(const string& input){
    DIR* dir = opendir(input.c_str());
        if (!dir) {
            cerr << "Erro abrindo a pasta: " << input << "\n";
            return;
        }

        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            string filename = entry->d_name;
            if (filename.find(".fit") != string::npos || 
                filename.find(".fits") != string::npos) {
                string fullPath = input + "/" + filename;
                Mat cleaned = processarImagem(fullPath);
                cleanedImages.push_back(cleaned);
            }
        }
        closedir(dir);
}

void AstroImagemProcessor::salvarPreview(const string& output){
    Mat merged = mesclarImagens();
        if (!merged.empty()) {
            //Salva versão de visualização
            Mat normalized;
            normalize(merged, normalized, 0, 255, NORM_MINMAX, CV_8U);
            imwrite(output + "/merged_preview.png", normalized);
        }
}

Mat AstroImagemProcessor::detectarLinhas(const Mat& image) {
    static int maskCounter = 0;
    maskCounter++;
    Mat gray;
    
    // Converte pra cinza
    if (image.channels() == 3) {
        cvtColor(image, gray, COLOR_BGR2GRAY);
    } else if (image.channels() == 1) {
        if (image.depth() != CV_8U) {
            double minVal, maxVal;
            minMaxLoc(image, &minVal, &maxVal);
            image.convertTo(gray, CV_8UC1, 255.0/(maxVal-minVal), -minVal*255.0/(maxVal-minVal));
        } else {
            gray = image.clone();
        }
    } 

    // Garante que tá no tipo certo por Line Segment Detector
    if (gray.type() != CV_8UC1) {
        gray.convertTo(gray, CV_8UC1);
    }
    
    // Cria o LSD
    Ptr<LineSegmentDetector> lsd = createLineSegmentDetector(
        LSD_REFINE_ADV, LSD_SCALE, LSD_SIGMA,
        2, LSD_ANG_TH, LSD_LOG_EPS, LSD_LOG_EPS
    );

    multiply(gray, 3, gray);  
    
    vector<Vec4f> lines;
    lsd->detect(gray, lines);
    
    // Create a máscara com as linhas
    Mat mask = Mat::zeros(image.size(), CV_8UC1);
    for(const auto& segment : lines) { 
        Point pt1(cvRound(segment[0]), cvRound(segment[1]));
        Point pt2(cvRound(segment[2]), cvRound(segment[3]));
        
        cv::line(mask, pt1, pt2, Scalar(255), LINE_MASK_WIDTH);
    }

    if(DEBUG){
        stringstream ss_mask;
        ss_mask << "./debug/debug_" << setw(4) << setfill('0') << maskCounter << "_mask.jpg";
        imwrite(ss_mask.str(), mask);
        cout << "Imagem " << maskCounter << " processada!" << '\n';
    }
    
    
    return mask;
}

Mat AstroImagemProcessor::encontrarRastros(const vector<Mat>& individual_images, 
                        const Mat& merged_image) {
    // 1. Verifica as entradas
    if(merged_image.empty()) {
        cerr << "Imagem mesclada está vazia!" << endl;
        return Mat();
    }
    
    // 2. Inicializa a mascára com as linhas de ruído
    Mat master_noise_mask = Mat::zeros(merged_image.size(), CV_8UC1);
    
    // 3. Processa as imagens individuais
    for(const auto& img : individual_images) {
        if(img.empty()) {
            cerr << "Pulando imagem vazia" << endl;
            continue;
        }
        
        Mat individual_mask = detectarLinhas(img);
        bitwise_or(master_noise_mask, individual_mask, master_noise_mask);
        
    }

    // 4. Processa a imagem mesclada
    Mat merged_mask = detectarLinhas(merged_image);
    
    // 5. Subtrai as linhas do ruido com as linhas da imagem mesclada
    Mat final_mask;
    cv::subtract(merged_mask, master_noise_mask, final_mask, noArray(), CV_16U);
    final_mask.convertTo(final_mask, CV_8U, 1.0/255.0);

    if(DEBUG){
        imwrite("./debug/noiseMask.jpg", master_noise_mask);
        imwrite("./debug/mergedMask.jpg", merged_mask);
        imwrite("./debug/finalMask.jpg", final_mask);
    }
    
    return final_mask;
}

