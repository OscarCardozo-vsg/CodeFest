#include "encrypt.h"
#include <Magick++.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib> 
#include <ctime>   

using namespace std;
using namespace Magick;

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
    string path = "";
    srand(static_cast<unsigned>(time(0)));
    int numName = rand() % 10000000000000;
    string numNameStr = to_string(numName);

    if(!outPath.empty()){
        path = outPath + "/img" + numNameStr + ".txt";
    }else{
        path = "img" + numNameStr + ".txt";
    }
    ofstream outfile(path);

    if (!outfile) {
        cerr << "No se pudo guardar el encriptado en: " << path << endl;
        return;
    }

    outfile << width << endl;
    outfile << height << endl;
    outfile << channels << std::endl;

    for (const unsigned char& val : bits) {
        outfile << static_cast<int>(val) << endl;
    }   

    outfile.close();
    std::cout << "Vector guardado en el archivo: " << path << endl;
}