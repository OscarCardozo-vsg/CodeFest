#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <string>
#include <vector>

int encryptImage(const std::string& path, const std::string& outPath);

int imageToBits(const std::string& path, const std::string& outPath);

void saveEncrypt(const std::vector<unsigned char>& bits, int width, int height, int channels, const std::string& outPath);

std::vector<unsigned char> encrypt(const std::vector<unsigned char>& plaintext, const unsigned char* key, const unsigned char* iv);

#endif
