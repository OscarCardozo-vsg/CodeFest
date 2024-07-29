#include "encrypt.h"
#include <Magick++.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <cstdlib> 
#include <ctime>   

using namespace std;
using namespace Magick;

int encryptImage(const string& path, const string& outPath);
int imageToBits(const string& path, const string& outPath);
void saveEncrypt(const vector<unsigned char>& bits, int width, int height, int channels, const string& outPath);
vector<unsigned char> encrypt(const vector<unsigned char>& plaintext, const unsigned char* key, const unsigned char* iv);

int encryptImage(const string& path, const string& outPath) {
    imageToBits(path, outPath);
    return 0;
}

int imageToBits(const string& path, const string& outPath) {
    InitializeMagick(nullptr);
    Image image;
    try {
        image.read(path);
    } catch (Exception &error_) {
        cerr << "Error: No se pudo cargar la imagen: " << error_.what() << endl;
        return -1;
    }

    int width = image.columns();
    int height = image.rows();
    int channels = image.channels();
    vector<unsigned char> bits(image.getPixels(0, 0, width, height), image.getPixels(0, 0, width, height) + width * height * channels);

    saveEncrypt(bits, width, height, channels, outPath);
    return 0;
}

void saveEncrypt(const vector<unsigned char>& bits, int width, int height, int channels, const string& outPath) {
    // Generar clave y vector de inicialización
    unsigned char key[32];
    unsigned char iv[16];

    if (!RAND_bytes(key, sizeof(key)) || !RAND_bytes(iv, sizeof(iv))) {
        cerr << "Error: No se pudo generar la clave y el vector de inicialización" << endl;
        return;
    }
    
    vector<unsigned char> encryptedBits = encrypt(bits, key, iv);
    
    //Ahora lo que hacemos es guardar los bits en un archivo
    string path = "";
    srand(static_cast<unsigned>(time(0)));
    int numName = rand() % 10000000000000;
    string numNameStr = to_string(numName);

    if (!outPath.empty()) {
        path = outPath + "/images" + numNameStr + ".txt";
    } else {
        path = "img" + numNameStr + ".txt";
    }
    ofstream outfile(path, ios::binary);

    if (!outfile){
        cerr << "No se pudo guardar el encriptado en: " << path << endl;
        return;
    }
    
    // Guardar clave y vector de inicialización en un archivo
    outfile.write(reinterpret_cast<const char*>(key), sizeof(key));
    outfile.write(reinterpret_cast<const char*>(iv), sizeof(iv));

    // Guardamos las dimensiones de la imagen
    outfile.write(reinterpret_cast<const char*>(&width), sizeof(width));
    outfile.write(reinterpret_cast<const char*>(&height), sizeof(height));
    outfile.write(reinterpret_cast<const char*>(&channels), sizeof(channels));

    // Ahora guardamos los datos encriptados
    outfile.write(reinterpret_cast<const char*>(encryptedBits.data()), encryptedBits.size());
    outfile.close();

    cout << "Vector guardado en el archivo:" << path << endl;
}

vector<unsigned char> encrypt(const vector<unsigned char>& plaintext, const unsigned char* key, const unsigned char* iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        cerr << "Error: No se pudo crear el contexto de encriptacion." << endl;
        return {};
    }
    
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        cerr << "Error: No se pudo inicializar la encriptacion." << endl;
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }

    vector<unsigned char> ciphertext(plaintext.size() + AES_BLOCK_SIZE);
    int len;
    int ciphertext_len;

    if (1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(), plaintext.size())) {
        cerr << "Error: No se pudo encriptar los datos" << endl;
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len)) {
        cerr << "Error: No se pudo finalizar la encriptacion." << endl;
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }
    ciphertext_len += len;

    ciphertext.resize(ciphertext_len);
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext;
}