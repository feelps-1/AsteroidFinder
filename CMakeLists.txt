cmake_minimum_required(VERSION 3.10)
project(FITS_OpenCV)

find_package(OpenCV REQUIRED)

set(CFITSIO_INCLUDE_DIR /usr/local/include)
set(CFITSIO_LIBRARY /usr/local/lib/libcfitsio.so)

set(CCFITS_INCLUDE_DIR /usr/local/include)
set(CCFITS_LIBRARY /usr/local/lib/libCCfits.so)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/usr/local")

include_directories(${CCFITS_INCLUDE_DIR})
link_directories(/usr/local/lib)

include_directories(${OpenCV_INCLUDE_DIRS} ${CFITSIO_INCLUDE_DIR})

add_executable(fits_opencv src/fits_opencv.cpp)

target_link_libraries(fits_opencv ${OpenCV_LIBS} ${CFITSIO_LIBRARY} ${CCFITS_LIBRARY})
