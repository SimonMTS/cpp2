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

wavParser::wavParser(const string& filename) {
    // open file
    this->file = make_unique<ifstream>();
    this->file->open(filename, ios::in | ios::binary);

    if (!this->file->good()) {
        std::cerr << "Could not open file.\n";
        exit(1);
    }
}

void wavParser::getData() const {
    string info = "fileType: wav\n";
    string chunks = "chunks:\n";

    // skip RIFF header
    char buffer[12];
    this->file->read(buffer, 12);

    auto bvData = make_unique<bitVector>();
    bool readingData = false;
    int bytesToRead = 4;

    char chunkName[5];
    uint32_t chunksize;
    uint16_t sampleSize;
    while (this->file->read(chunkName, bytesToRead)) {
        if (!readingData) {
            chunkName[4] = '\0';
            this->file->read((char*)&chunksize, 4);
            chunks += "   " + string(chunkName) + " - " +
                      std::to_string(chunksize) + "\n";

            if (strcmp(chunkName, "fmt ") == 0) {
                char sBuff[16];
                this->file->read(sBuff, 16);

                sampleSize = (uint16_t)sBuff[14];
                info += "sampleSize: " + std::to_string(sampleSize) + "bit\n";
                continue;
            }

            if (strcmp(chunkName, "data") == 0) {
                readingData = true;
                bytesToRead = sampleSize / 8;
                continue;
            }

            this->file->seekg(chunksize, std::ios_base::cur);
        } else {
            bvData->pushFinal(chunkName[0]);
        }
    }

    std::cerr << info;
    std::cerr << chunks;
    std::cerr << "secret:\n";
    std::cout << bvData->toString() << std::endl;
}

void wavParser::setData(const string& data) const {
    string info = "fileType: wav\n";
    string chunks = "chunks:\n";

    // skip RIFF header
    char buffer[12];
    this->file->read(buffer, 12);
    std::cout << buffer[0] << buffer[1] << buffer[2] << buffer[3] << buffer[4]
              << buffer[5] << buffer[6] << buffer[7] << buffer[8] << buffer[9]
              << buffer[10] << buffer[11];

    auto bvData = make_unique<bitVector>(data);
    bool readingData = false;
    int bytesToRead = 4;

    char chunkName[5];
    uint32_t chunksize;
    uint16_t sampleSize;
    while (this->file->read(chunkName, bytesToRead)) {
        if (!readingData) {
            chunkName[4] = '\0';
            this->file->read((char*)&chunksize, 4);
            chunks += "   " + string(chunkName) + " - " +
                      std::to_string(chunksize) + "\n";

            std::cout << chunkName[0] << chunkName[1] << chunkName[2]
                      << chunkName[3];

            std::cout << (char)(chunksize >> 0);
            std::cout << (char)(chunksize >> 8);
            std::cout << (char)(chunksize >> 16);
            std::cout << (char)(chunksize >> 24);

            if (strcmp(chunkName, "fmt ") == 0) {
                char sBuff[16];
                this->file->read(sBuff, 16);
                std::cout << sBuff[0] << sBuff[1] << sBuff[2] << sBuff[3]
                          << sBuff[4] << sBuff[5] << sBuff[6] << sBuff[7]
                          << sBuff[8] << sBuff[9] << sBuff[10] << sBuff[11]
                          << sBuff[12] << sBuff[13] << sBuff[14] << sBuff[15];

                sampleSize = (uint16_t)sBuff[14];
                info += "sampleSize: " + std::to_string(sampleSize) + "bit\n";
                continue;
            }

            if (strcmp(chunkName, "data") == 0) {
                readingData = true;
                bytesToRead = sampleSize / 8;
                continue;
            }

            ulong i = 0;
            for (i = 0; i < chunksize; i++) {
                this->file->read(chunkName, 1);
                std::cout << chunkName[0];
            }
            // std::cerr << chunksize << "\n";
            // std::cerr << i << "\n";
            // this->file->seekg(chunksize, std::ios_base::cur);
        } else {
            // bvData->addFinal(chunkName[0]);
            if (bvData->pop()) {
                chunkName[0] |= 1;
            } else {
                chunkName[0] &= ~1;
            }

            if (sampleSize == 16) {
                std::cout << chunkName[0] << chunkName[1];
            } else {
                std::cout << chunkName[0] << chunkName[1] << chunkName[2];
            }
        }
    }

    std::cerr << info;
    std::cerr << chunks;
}