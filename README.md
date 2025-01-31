# Asteroid Finder - NEOSSAT

Este projeto é um script em C++ que utiliza técnicas de visão computacional, detecção de linhas e processamento de imagem para buscar rastros de asteroides em datasets do satélite canadense NEOSSAT. O processamento inclui remoção de ruído eletrônico, limpeza da imagem com a área de overscan, e mesclagem de múltiplas imagens para realçar os rastros dos asteroides.

## 📜 Referências Científicas
O projeto é baseado em técnicas apresentadas nos seguintes artigos:

1. [**"A COMPUTER VISION APPROACH FOR DETECTION OF ASTEROIDS/COMETS IN SPACE SATELLITE IMAGES"**](https://ui.adsabs.harvard.edu/abs/2020ISPAr43B3.1185Y/abstract)
   - Autores: Bahareh Yekkehkhany, Parnia Shokri, Amin Zadeh
2. [**"THE NEOSSAT EXPERIENCE: 5 YEARS IN THE LIFE OF CANADA’S SPACE SURVEILLANCE TELESCOPE"**](https://conference.sdo.esoc.esa.int/proceedings/neosst1/paper/494)
   - Autores: V. Abbasi, S. Thorsteinson, D. Balam, J. Rowe, D. Laurin, L. Scott, M. Doyon

## 📂 Estrutura do Projeto
O código-fonte é dividido em dois arquivos principais:

- `astro_image.cpp`: Classe responsável pela limpeza das imagens usando a área de overscan e remoção de ruído eletrônico via FFT.
- `processor.cpp`: Classe que lida com a detecção dos rastros de asteroides.
- `fits_opencv.cpp`: Arquivo principal que executa o pipeline de processamento.

## 🛠️ Dependências
Para compilar e executar o projeto, instale as seguintes bibliotecas:

- **OpenCV** (para processamento de imagem)
- **CFITSIO** (para manipulação de arquivos FITS)
- **CCfits** (interface C++ para CFITSIO)
- **zlib** (compressão de dados, necessária para CFITSIO)
- **CMake** (para gerenciar a compilação)

### 🔧 Instalação das Dependências (Ubuntu/Debian)
```bash
sudo apt update && sudo apt install -y libopencv-dev libcfitsio-dev libccfits-dev zlib1g-dev cmake g++
```

## 🚀 Compilação e Execução

Nesse repositório existem dois datasets de exemplo do NEOSSAT: **292** e **306**!

### 1️⃣ Compilar o Projeto
```bash
mkdir build && cd build
cmake ..
make
```
### 2️⃣ Executar o Script
```bash
./fits_opencv <pasta_com_imagens> <pasta_de_saida>
```
- `<pasta_com_imagens>`: Diretório contendo os arquivos FITS do NEOSSAT.
- `<pasta_de_saida>`: Diretório onde os resultados processados serão salvos.

## 🔬 Metodologia
1. **Pré-processamento**:
   - Carregamento das imagens FITS do dataset do NEOSSAT.
   - Remoção do viés da imagem usando a área de overscan.
   - Redução do ruído eletrônico através da Transformada Rápida de Fourier (FFT).
2. **Realce de Rastros**:
   - Mesclagem de múltiplas imagens para tornar os rastros mais evidentes.
   - Comparação entre imagens individuais e a imagem mesclada para remoção de ruído adicional.
3. **Detecção de Linhas**:
   - Aplicação de algoritmos de detecção de linhas para identificar os rastros dos asteroides.

## 📷 Resultado Esperado
O script gera uma imagem processada destacando os rastros detectados dos asteroides, facilitando sua análise e identificação.

Abaixo está a comparação do resultado das técnicas do artigo com o script processando o dataset do dia 306 de 2019, ambas as imagens já estão mescladas, existem linhas que são ruídos que só aparecem nas
imagens individuais.

<p align="center">
  <img src="https://github.com/user-attachments/assets/a4738af4-ec9a-4545-8d7e-171e9b606522" width="400" alt="Imagem retirada do artigo">
</p>
<p align="center"><em>Imagem retirada do artigo</em></p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/3896b8b3-462c-421f-8134-5d00ff74011d" width="400" alt="Imagem resultante do script com apenas 1 falso positivo">
</p>
<p align="center"><em>Imagem resultante do script com apenas 1 falso positivo gerada pela borda de uma estrela</em></p>

 - Existem melhorias a serem feitas no projeto como a melhoria da limpeza de ruído em diferentes datasets, além de melhoria da visualização e contraste
---

### 📌 Contribuições
Sinta-se à vontade para contribuir com melhorias no código ou na documentação. Sugestões e pull requests são bem-vindos!

### 📝 Licença
Este projeto é de código aberto e pode ser utilizado livremente para fins educacionais e científicos.

