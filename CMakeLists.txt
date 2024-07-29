cmake_minimum_required(VERSION 3.10)
project(CodeFest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Configuración de ImageMagick
find_package(ImageMagick COMPONENTS Magick++ REQUIRED)
include_directories(${ImageMagick_INCLUDE_DIRS})

# Configuración manual de OpenSSL
set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib/VC/x64/MTd/libcrypto.lib")
set(OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib/VC/x64/MTd/libssl.lib")

# Verificar que los archivos existen
if(NOT EXISTS ${OPENSSL_CRYPTO_LIBRARY})
    message(FATAL_ERROR "OpenSSL crypto library not found at ${OPENSSL_CRYPTO_LIBRARY}")
endif()
if(NOT EXISTS ${OPENSSL_SSL_LIBRARY})
    message(FATAL_ERROR "OpenSSL SSL library not found at ${OPENSSL_SSL_LIBRARY}")
endif()

# Configurar OpenSSL manualmente
add_library(OpenSSL::Crypto STATIC IMPORTED)
set_target_properties(OpenSSL::Crypto PROPERTIES
    IMPORTED_LOCATION "${OPENSSL_CRYPTO_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}"
)

add_library(OpenSSL::SSL STATIC IMPORTED)
set_target_properties(OpenSSL::SSL PROPERTIES
    IMPORTED_LOCATION "${OPENSSL_SSL_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}"
)

include_directories(${OPENSSL_INCLUDE_DIR})

set(IMAGES_DIR "${CMAKE_SOURCE_DIR}/imagenes")
set(ENCRYPTED_DIR "${CMAKE_SOURCE_DIR}/encrypted")

add_definitions(-DIMAGES_DIR=\"${IMAGES_DIR}\")
add_definitions(-DENCRYPTED_DIR=\"${ENCRYPTED_DIR}\")

add_executable(CodeFest main.cpp encrypt.cpp decrypt.cpp)
target_link_libraries(CodeFest ${ImageMagick_LIBRARIES} OpenSSL::Crypto OpenSSL::SSL)

# Imprimir información de depuración
message(STATUS "OPENSSL_ROOT_DIR: ${OPENSSL_ROOT_DIR}")
message(STATUS "OPENSSL_INCLUDE_DIR: ${OPENSSL_INCLUDE_DIR}")
message(STATUS "OPENSSL_CRYPTO_LIBRARY: ${OPENSSL_CRYPTO_LIBRARY}")
message(STATUS "OPENSSL_SSL_LIBRARY: ${OPENSSL_SSL_LIBRARY}")