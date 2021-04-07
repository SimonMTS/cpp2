#include "cafParser.hpp"

#include <bits/stdint-uintn.h>
#include <stdint.h>
#include <sys/types.h>

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

// https://developer.apple.com/library/archive/documentation/MusicAudio/Reference/CAFSpec/CAF_spec/CAF_spec.html#//apple_ref/doc/uid/TP40001862-CH210-TPXREF101

cafParser::cafParser(const string& filename) {
    // open file
    this->file = make_unique<ifstream>();
    this->file->open(filename, ios::in | ios::binary);

    if (!this->file->good()) {
        std::cerr << "Could not open file.\n";
        exit(1);
    }
}

void cafParser::getData() const {
    string info = "fileType: caf\n";
    string chunks = "chunks:\n";

    // char array to read data into
    char buffer[16];

    // skip CAFFileHeader header
    this->file->read(buffer, 8);
    // todo, is this really a caf file?

    auto bvData = make_unique<bitVector>();
    bool readingData = false;
    int bytesToRead = 4;

    int64_t chunksize;
    uint16_t sampleSize;
    while (this->file->read(buffer, bytesToRead)) {
        if (!readingData) {
            buffer[4] = '\0';  // end string after 4 chars

            {
                char csBuffer[8];
                this->file->read(csBuffer, 8);
                chunksize = ((int64_t)csBuffer[7] << 0) +   //
                            ((int64_t)csBuffer[6] << 8) +   //
                            ((int64_t)csBuffer[5] << 16) +  //
                            ((int64_t)csBuffer[4] << 24) +  //
                            ((int64_t)csBuffer[3] << 32) +  //
                            ((int64_t)csBuffer[2] << 40) +  //
                            ((int64_t)csBuffer[1] << 48) +  //
                            ((int64_t)csBuffer[0] << 56);   //
            }
            chunks += "   " + string(buffer) + " - " +
                      std::to_string(chunksize) + "\n";

            if (strcmp(buffer, "desc") == 0) {
                this->file->read(buffer, 32);

                sampleSize = 16;
                // sampleSize = (uint16_t)buffer[14];
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

void cafParser::setData(const string& data) const {
    string info = "fileType: caf\n";
    string chunks = "chunks:\n";

    // char array to read data into
    char buffer[16];

    // skip RIFF header
    this->file->read(buffer, 12);
    binToStdout(buffer, 12);

    auto bvData = make_unique<bitVector>(data);
    bool readingData = false;
    int bytesToRead = 4;

    uint32_t chunksize;
    uint16_t sampleSize;
    while (this->file->read(buffer, bytesToRead)) {
        if (!readingData) {
            buffer[4] = '\0';
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
