cmake_minimum_required(VERSION 3.10)
project(CodeFest)

# Requiere C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define variables for include and library directories
set(IMAGEMAGICK_INCLUDE_DIR "${IMAGEMAGICK_ROOT}/include")
set(IMAGEMAGICK_LIB_DIR "${IMAGEMAGICK_ROOT}/lib")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT}/include")
set(OPENSSL_LIB_DIR "${OPENSSL_ROOT}/lib/VC/x64/MTd")

# Agregar los directorios de inclusión
include_directories(${IMAGEMAGICK_INCLUDE_DIR})
include_directories(${OPENSSL_INCLUDE_DIR})

# Agregar los directorios de búsqueda de bibliotecas
link_directories(${IMAGEMAGICK_LIB_DIR})
link_directories(${OPENSSL_LIB_DIR})

# Agregar el ejecutable
add_executable(CodeFest main.cpp encrypt.cpp decrypt.cpp)

# Enlazar las bibliotecas
target_link_libraries(CodeFest
    "CORE_RL_Magick++_.lib"
    "CORE_RL_MagickCore_.lib"
    "CORE_RL_MagickWand_.lib"
    "libcrypto.lib"
    "libssl.lib"
    ws2_32
    gdi32
    kernel32
    user32
    winspool
    shell32
    ole32
    oleaut32
    uuid
    comdlg32
    advapi32
)
