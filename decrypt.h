#ifndef DECRYPT_H
#define DECRYPT_H

#include <string>
#include <vector>

int decryptImage(const std::string& path, const std::string& outPath);

std::string getName(const std::string& name);

std::vector<int> txtToVector(const std::string& path);

void vectorToImage(const std::vector<int>& data, const std::string& outPath, const std::string& name);

#endif