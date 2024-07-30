cmake_minimum_required(VERSION 3.10)
project(CodeFest)

# Requiere C++11
set(CMAKE_CXX_STANDARD 11)

# Agregar los directorios de inclusión
include_directories("C:/Program Files/ImageMagick-7.1.1-Q16-HDRI/include")
include_directories("C:/Program Files/OpenSSL-Win64/include")

# Agregar los directorios de búsqueda de bibliotecas
link_directories("C:/Program Files/ImageMagick-7.1.1-Q16-HDRI/lib")
link_directories("C:/Program Files/OpenSSL-Win64/lib/VC/x64/MTd")

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
