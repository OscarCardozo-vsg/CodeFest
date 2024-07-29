#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <string>
#include <Magick++.h>
#include "decrypt.h"

using namespace std;
using namespace Magick;

int decryptImage(const string& path, const string& outPath) {
    string name = getName(path);
    std::vector<int> data = txtToVector(path);
    vectorToImage(data, outPath, name);
    return 0;
}

string getName(const string& name) {
    size_t lastSlashIdx = name.find_last_of("\\/");
    if (string::npos != lastSlashIdx) {
        return name.substr(lastSlashIdx + 1);
    }
    return name;
}

std::vector<int> txtToVector(const string& path) {
    std::vector<int> data;
    ifstream infile(path);
    if (!infile) {
        cerr << "No se pudo abrir el archivo: " << path << endl;
        return data;
    }

    int value;
    while (infile >> value){
        data.push_back(value);
    }
    infile.close();
    return data;
}

void vectorToImage(const std::vector<int>& data, const string& outPath, const string& name) {
    string finalPath;
    int width = data[0];
    int height = data[1];
    int channels = data[2];

    if (data.size() < 3 + width * height * channels) {
        cerr << "El archivo fue alterado o corrupto. (Tamaño de la imagen no coinciden)" << endl;
        return;
    }

    Image image(Geometry(width, height), ColorRGB(0, 0, 0));
    image.type(TrueColorType);
    auto it = data.begin() + 3;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            ColorRGB color;
            if (channels == 3) {
                color.red(*it++ / 255.0);
                color.green(*it ++ / 255.0);
                color.blue(*it ++ / 255.0);
            } else if (channels == 1) {
                double gray = *it++ / 255.0;
                color.red(gray);
                color.green(gray);
                color.blue(gray);
            }
            image.pixelColor(x, y, color);
        }
    }

    if(!outPath.empty()){
        finalPath = outPath + "/" + name + ".png";
    }else{
        finalPath = name + ".png";
    }
    image.write(finalPath);
    cout << "Imagen guardada en: " + finalPath << endl;
}
    