#include "wavParser.hpp"

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
            // std::cout << chunkName << " - " << chunksize << std::endl;

            if (strcmp(chunkName, "data") == 0) {
                readingData = true;
                bytesToRead = 2;
                continue;
            }

            file.seekg(chunksize, std::ios_base::cur);
        } else {
            bvData->addFinal(chunkName[0]);
        }
    }

    return bvData->toString();
}

void wavParser::setData(const string& filename, const string& data) {
    // auto bvData = make_unique<bitVector>();

    // std::cout << bvData->getBits();
}