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

    // char array to read data into
    char buffer[16];

    // skip RIFF header
    this->file->read(buffer, 12);
    buffer[12] = '\0';  // end string after 4 chars
    if (strcmp(&buffer[8], "WAVE") != 0) {
        std::cerr << "Are you sure this is a wav file?\n\n";
    }

    auto bvData = make_unique<bitVector>();
    bool readingData = false;
    int bytesToRead = 4;

    uint32_t chunksize;
    uint16_t sampleSize;
    while (this->file->read(buffer, bytesToRead)) {
        if (!readingData) {
            buffer[4] = '\0';  // end string after 4 chars
            this->file->read((char*)&chunksize, 4);
            chunks += "   " + string(buffer) + " - " +
                      std::to_string(chunksize) + "\n";

            if (strcmp(buffer, "fmt ") == 0) {
                this->file->read(buffer, 16);

                sampleSize = (uint16_t)buffer[14];
                info += "sampleSize: " + std::to_string(sampleSize) + "bit\n";
                continue;
            }

            if (strcmp(buffer, "data") == 0) {
                readingData = true;
                bytesToRead = sampleSize / 8;
                continue;
            }

            this->file->seekg(chunksize, std::ios_base::cur);
        } else {
            bvData->pushFinal(buffer[0]);
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

    // char array to read data into
    char buffer[16];

    // skip RIFF header
    this->file->read(buffer, 12);
    buffer[12] = '\0';  // end string after 4 chars
    if (strcmp(&buffer[8], "WAVE") != 0) {
        std::cerr << "Are you sure this is a wav file?\n\n";
    }
    binToStdout(buffer, 12);

    auto bvData = make_unique<bitVector>(data);
    bool readingData = false;
    int bytesToRead = 4;

    uint32_t chunksize;
    uint16_t sampleSize;
    while (this->file->read(buffer, bytesToRead)) {
        if (!readingData) {
            buffer[4] = '\0';  // end string after 4 chars
            binToStdout(buffer, 4);

            this->file->read((char*)&chunksize, 4);
            chunks += "   " + string(buffer) + " - " +
                      std::to_string(chunksize) + "\n";
            binToStdout((char*)&chunksize, 4);

            if (strcmp(buffer, "fmt ") == 0) {
                this->file->read(buffer, 16);
                binToStdout(buffer, 16);

                sampleSize = (uint16_t)buffer[14];
                info += "sampleSize: " + std::to_string(sampleSize) + "bit\n";
                continue;
            }

            if (strcmp(buffer, "data") == 0) {
                readingData = true;
                bytesToRead = sampleSize / 8;
                continue;
            }

            ulong i = 0;
            for (i = 0; i < chunksize; i++) {
                this->file->read(buffer, 1);
                binToStdout(buffer, 1);
            }
        } else {
            if (bvData->pop()) {
                buffer[0] |= 1;
            } else {
                buffer[0] &= ~1;
            }
            binToStdout(buffer, sampleSize == 16 ? 2 : 3);
        }
    }

    std::cerr << info;
    std::cerr << chunks;
}
