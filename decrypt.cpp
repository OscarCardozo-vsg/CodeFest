#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <string>
#include <Magick++.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include "decrypt.h"

using namespace std;
using namespace Magick;

int decryptImage(const string& path, const string& outPath);
string getName(const string& name);
std::vector<unsigned char> txtToVector(const string& path, unsigned char* key, unsigned char* iv, int& width, int& height, int& channels);
void vectorToImage(const std::vector<unsigned char>& data, int width, int height, int channels, const string& outPath, const string& name);
std::vector<unsigned char> decrypt(const std::vector<unsigned char>& ciphertext, const unsigned char* key, const unsigned char* iv);

int decryptImage(const string& path, const string& outPath) {
    string name = getName(path);
    unsigned char key[32];
    unsigned char iv[16];
    int width, height, channels;
    std::vector<unsigned char> data = txtToVector(path, key, iv, width, height, channels);
    vectorToImage(data, width, height, channels, outPath, name);
    return 0;
}

string getName(const string& name) {
    size_t lastSlashIdx = name.find_last_of("\\/");
    if (string::npos != lastSlashIdx) {
        return name.substr(lastSlashIdx + 1);
    }
    return name;
}

std::vector<unsigned char> txtToVector(const string& path, unsigned char* key, unsigned char* iv, int& width, int& height, int& channels) {
    std::vector<unsigned char> data;
    ifstream infile(path, ios::binary);
    if (!infile) {
        cerr << "No se pudo abrir el archivo: " << path << endl;
        return data;
    }

    // Leer clave y vector de inicialización
    infile.read(reinterpret_cast<char*>(key), 32);
    infile.read(reinterpret_cast<char*>(iv), 16);

    // Leer dimensiones de la imagen
    infile.read(reinterpret_cast<char*>(&width), sizeof(int));
    infile.read(reinterpret_cast<char*>(&height), sizeof(int));
    infile.read(reinterpret_cast<char*>(&channels), sizeof(int));

    // Leer datos encriptados
    infile.seekg(0, ios::end);
    size_t fileSize = infile.tellg();
    infile.seekg(32 + 16 + 3 * sizeof(int));
    size_t dataSize = fileSize - (32 + 16 + 3 * sizeof(int));
    std::vector<unsigned char> encryptedData(dataSize);
    infile.read(reinterpret_cast<char*>(encryptedData.data()), dataSize);
    infile.close();

    // Desencriptar datos
    data = decrypt(encryptedData, key, iv);
    return data;
}

void vectorToImage(const std::vector<unsigned char>& data, int width, int height, int channels, const std::string& outPath, const std::string& name) {
    if (data.size() < width * height * channels) {
        cerr << "El archivo fue alterado o corrupto. (Tamaño de la imagen no coinciden)" << endl;
        return;
    }

    Image image(Geometry(width, height), ColorRGB(0, 0, 0));
    image.type(TrueColorType);
    auto it = data.begin();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            ColorRGB color;
            if (channels == 3) {
                color.red(*it++ / 255.0);
                color.green(*it++ / 255.0);
                color.blue(*it++ / 255.0);
            } else if (channels == 1) {
                double gray = *it++ / 255.0;
                color.red(gray);
                color.green(gray);
                color.blue(gray);
            }
            image.pixelColor(x, y, color);
        }
    }

    std::string finalPath;
    if(!outPath.empty()){
        finalPath = outPath + "/" + name + ".png";
    } else {
        finalPath = name + ".png";
    }
    image.write(finalPath);
    cout << "Imagen guardada en: " + finalPath << endl;
}

std::vector<unsigned char> decrypt(const std::vector<unsigned char>& ciphertext, const unsigned char* key, const unsigned char* iv){
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx){
        cerr << "Error: No se logro crear el contexto de desencriptación." << endl;
        return {};
    }

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)){
        cerr << "Error: No se pudo inicializar la desencriptación." << endl;
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    
    std::vector<unsigned char> plaintext(ciphertext.size() + AES_BLOCK_SIZE);
    int len;
    int plaintext_len;

    if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size())){
        cerr << "Error: No se ha podido desencriptar los datos." << endl;
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    
    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len)){
        cerr << "Error: No se pudo finalizar la desencriptación." << endl;
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    
    plaintext_len += len;

    plaintext.resize(plaintext_len);
    EVP_CIPHER_CTX_free(ctx);

    return plaintext;
}
    