# Relatório de Funcionalidades POO

## 1. Introdução

Este relatório descreve as funcionalidades desenvolvidas para o projeto de processamento de imagens astronômicas no formato FITS e detecção de rastro de asteroides, utilizando conceitos de **Programação Orientada a Objetos (POO)** em **C++**. O projeto implementa classes e métodos para manipulação e análise dessas imagens, aplicando técnicas de processamento digital.

---
## 2. Arquitetura de Classes
A abstração é utilizada para esconder detalhes complexos de manipulação de imagens e processamento de dados, expondo apenas as funcionalidades essenciais.
### 2.1 Classe `AstroImagem`

A classe `AstroImagem` representa uma imagem FITS e encapsula sua leitura, tratamento e conversão para **OpenCV**. 
#### **Atributos:**

```cpp
private:
    string imgPath;          // Caminho do arquivo FITS
    int largura, altura;     // Dimensões da imagem
    Mat imagem;              // Matriz OpenCV da imagem
    string timestamp;        // Data e hora da observação
```

#### **Método Construtor:**

O construtor abre o arquivo FITS, extrai os dados e converte para uma matriz OpenCV.

```cpp
AstroImagem::AstroImagem(const string& filePath) : imgPath(filePath) {
    unique_ptr<FITS> pInFile(new FITS(filePath, Read, true));
    PHDU& imgHeader = pInFile->pHDU();

    altura = imgHeader.axis(1);
    largura = imgHeader.axis(0);
    
    valarray<float> imgData;
    imgHeader.read(imgData);

    Mat imagemOriginal(altura, largura, CV_32F);
    for(long linha=0; linha<altura; ++linha) {
        for(long col=0; col<largura; ++col) {
            imagemOriginal.at<float>(linha, col) = imgData[linha*largura + col];
        }
    }
    imagem = imagemOriginal;
}
```

#### **Método para Retornar a Imagem Processada:**

```cpp
Mat AstroImagem::getImage() const {
    return imagem;
}
```

---

### 2.2 Classe `AstroImagemProcessor`

Essa classe gerencia o processamento de um conjunto de imagens FITS, aplicando filtros e mesclando imagens.
#### **Atributos:**

```cpp
private:
    vector<Mat> cleanedImages; // Lista de imagens processadas
```
#### **Método para Processar uma Pasta de Imagens:**

```cpp
void AstroImagemProcessor::processarPasta(const string& input) {
    DIR* dir = opendir(input.c_str());
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        string filename = entry->d_name;
        if (filename.find(".fits") != string::npos) {
            string fullPath = input + "/" + filename;
            Mat cleaned = processarImagem(fullPath);
            cleanedImages.push_back(cleaned);
        }
    }
    closedir(dir);
}
```
#### **Método para Mesclar Imagens:**

```cpp
Mat AstroImagemProcessor::mesclarImagens() {
    if (cleanedImages.empty()) {
        cerr << "Nenhuma imagem processada." << endl;
        return Mat();
    }
    Mat merged = cleanedImages[0].clone();
    for (size_t i = 1; i < cleanedImages.size(); ++i) {
        max(merged, cleanedImages[i], merged);
    }
    return merged;
}
```

---
## 3. Encapsulamento

A implementação respeita o princípio de **encapsulamento**, protegendo os atributos e expondo apenas métodos necessários para interação. O projeto também encapsula funcionalidades relacionadas ao processamento de imagens em classes distintas. Isso garante que cada classe tenha uma responsabilidade bem definida, evitando a exposição desnecessária de detalhes internos.

- **Classe `AstroImagemProcessor`**: Responsável pelo processamento das imagens e pela aplicação das técnicas de fusão e limpeza. 
- **Classe de Limpeza de Imagem (`astro_image.cpp`)**: Especializada na remoção de ruídos e correção de overscan.

Exemplo de **getter**:

```cpp
string AstroImagem::getTimestamp() const {
    return timestamp;
}
```

Cada classe mantém seus atributos e métodos internos privados sempre que possível, garantindo um código mais seguro e menos propenso a erros.

---
## 4. Reuso de Código e Herança
Embora o projeto não utilize diretamente a herança entre classes definidas por mim, ele se beneficia da herança presente nas bibliotecas externas.
#### **Exemplo no Projeto** 
- O OpenCV utiliza classes base como `cv::Algorithm` para estender funcionalidades, permitindo que o código utilize métodos polimórficos para processamento de imagens. 
- A biblioteca **CCfits** também usa herança para organizar diferentes tipos de manipulação de arquivos FITS. Se o projeto for expandido, poderia ser implementada uma estrutura de herança para diferentes métodos de processamento de imagens.

---

## 5. Uso de Bibliotecas Orientadas a Objetos

O projeto utiliza bibliotecas que seguem os princípios de OOP, permitindo um desenvolvimento modular e eficiente.

### 5.1. OpenCV
- Usa classes como cv::Mat para representar imagens.
- Métodos encapsulados para aplicar transformadas de Fourier (cv::dft()).
- Herança e polimorfismo para diferentes métodos de filtragem e processamento.

### 5.2. CCfits e CFitsIO
- Organizam o acesso a arquivos FITS de forma orientada a objetos.
- Oferecem classes para ler, escrever e manipular metadados de imagens astronômicas.

### 5.3. STL (Standard Template Library)
- Usa **std::vector** para armazenar imagens processadas.
- **std::string** para manipulação de caminhos e nomes de arquivos.
- **std::unique_ptr** é usado para gerenciamento seguro de memória.

## 6. Gerenciamento de Memória

### **Uso do OpenCV (cv::Mat)**
- OpenCV gerencia automaticamente a alocação e desalocação de memória para imagens.
- Não há necessidade de desalocar manualmente os dados de imagens.

