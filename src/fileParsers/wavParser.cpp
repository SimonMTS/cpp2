#include "wavParser.hpp"

#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../helpers/bitVector.hpp"

using std::ifstream;
using std::ios;
using std::make_unique;

string wavParser::getData(const string& filename) {
    // open file
    ifstream file;
    file.open(filename);

    // skip RIFF header
    char buffer[12];
    file.read(buffer, 12);

    auto bvData = make_unique<bitVector>();
    bool readingData = false;
    int bytesToRead = 4;

    char chunkName[5];
    unsigned long chunksize;
    while (file.read(chunkName, bytesToRead)) {
        if (!readingData) {
            chunkName[4] = '\0';
            file.read((char*)&chunksize, 4);
            // std::cerr << chunkName << " - " << chunksize << std::endl;

            if (strcmp(chunkName, "data") == 0) {
                readingData = true;
                bytesToRead = 2;
                continue;
            }

            file.seekg(chunksize, std::ios_base::cur);
        } else {
            bvData->pushFinal(chunkName[0]);
        }
    }

    return bvData->toString();
}

void wavParser::setData(const string& filename, const string& data) {
    // open file
    ifstream file;
    file.open(filename);

    // skip RIFF header
    char buffer[12];
    file.read(buffer, 12);
    std::cout << buffer[0] << buffer[1] << buffer[2] << buffer[3] << buffer[4]
              << buffer[5] << buffer[6] << buffer[7] << buffer[8] << buffer[9]
              << buffer[10] << buffer[11];

    auto bvData = make_unique<bitVector>(data);
    bool readingData = false;
    int bytesToRead = 4;

    char chunkName[5];
    ulong chunksize;
    while (file.read(chunkName, bytesToRead)) {
        if (!readingData) {
            chunkName[4] = '\0';
            file.read((char*)&chunksize, 4);
            std::cerr << chunkName << " - " << chunksize << std::endl;

            std::cout << chunkName[0] << chunkName[1] << chunkName[2]
                      << chunkName[3];

            std::cout << (char)(chunksize >> 0);
            std::cout << (char)(chunksize >> 8);
            std::cout << (char)(chunksize >> 16);
            std::cout << (char)(chunksize >> 24);

            if (strcmp(chunkName, "data") == 0) {
                readingData = true;
                bytesToRead = 2;
                continue;
            }

            ulong i = 0;
            for (i = 0; i < chunksize; i++) {
                file.read(chunkName, 1);
                std::cout << chunkName[0];
            }
            std::cerr << chunksize << "\n";
            std::cerr << i << "\n";
            // file.seekg(chunksize, std::ios_base::cur);
        } else {
            // bvData->addFinal(chunkName[0]);
            if (bvData->pop()) {
                chunkName[0] |= 1;
            } else {
                chunkName[0] &= ~1;
            }

            std::cout << chunkName[0] << chunkName[1];
        }
    }
}