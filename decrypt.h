#ifndef DECRYPT_H
#define DECRYPT_H

#include <string>
#include <vector>

int decryptImage(const std::string& path, const std::string& outPath);

std::string getName(const std::string& name);

std::vector<int> txtToVector(const std::string& path);

void vectorToImage(const std::vector<unsigned char>& data, int width, int height, int channels, const std::string& outPath, const std::string& name);

std::vector<unsigned char> decrypt(const std::vector<unsigned char>& ciphertext, const unsigned char* iv);

#endif