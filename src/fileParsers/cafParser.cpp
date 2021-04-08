#include "cafParser.hpp"

#include <bits/floatn-common.h>
#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>
#include <stdint.h>
#include <stdlib.h>
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
    char buffer[32];

    // skip CAFFileHeader header
    this->file->read(buffer, 8);
    buffer[4] = '\0';  // end string after 4 chars
    if (strcmp(buffer, "caff") != 0) {
        std::cerr << "Are you sure this is a caf file?\n\n";
    }

    auto bvData = make_unique<bitVector>();
    bool readingData = false;
    int bytesToRead = 4;

    int64_t chunksize;
    _Float64 sampleSize;
    while (this->file->read(buffer, bytesToRead)) {
        if (!readingData) {
            buffer[4] = '\0';  // end string after 4 chars

            this->file->read(&buffer[24], 8);
            chunksize = ((uint8_t)buffer[31] << 0) |    //
                        ((uint16_t)buffer[30] << 8) |   //
                        ((uint64_t)buffer[29] << 16) |  //
                        ((uint64_t)buffer[28] << 24) |  //
                        ((uint64_t)buffer[27] << 32) |  //
                        ((uint64_t)buffer[26] << 40) |  //
                        ((uint64_t)buffer[25] << 48) |  //
                        ((uint64_t)buffer[24] << 56);   //
            chunks += "   " + string(buffer) + " - " +
                      std::to_string(chunksize) + "\n";

            if (strcmp(buffer, "desc") == 0) {
                this->file->read(buffer, 32);

                sampleSize = (uint16_t)buffer[31];
                info +=
                    "sampleSize: " + std::to_string((int)sampleSize) + "bit\n";
                continue;
            }

            if (strcmp(buffer, "data") == 0) {
                readingData = true;
                bytesToRead = sampleSize / 8;
                this->file->seekg(4, std::ios_base::cur);  // skip mEditCount
                continue;
            }

            this->file->seekg(chunksize, std::ios_base::cur);
        } else {
            bvData->pushFinal(buffer[1]);
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
    char buffer[32];

    // skip CAFFileHeader header
    this->file->read(buffer, 8);
    buffer[4] = '\0';  // end string after 4 chars
    if (strcmp(buffer, "caff") != 0) {
        std::cerr << "Are you sure this is a caf file?\n\n";
    }
    binToStdout(buffer, 8);

    auto bvData = make_unique<bitVector>(data);
    bool readingData = false;
    int bytesToRead = 4;

    int64_t chunksize;
    _Float64 sampleSize;
    while (this->file->read(buffer, bytesToRead)) {
        if (!readingData) {
            buffer[4] = '\0';  // end string after 4 chars
            binToStdout(buffer, 4);

            this->file->read(&buffer[24], 8);
            chunksize = ((uint8_t)buffer[31] << 0) |    //
                        ((uint16_t)buffer[30] << 8) |   //
                        ((uint64_t)buffer[29] << 16) |  //
                        ((uint64_t)buffer[28] << 24) |  //
                        ((uint64_t)buffer[27] << 32) |  //
                        ((uint64_t)buffer[26] << 40) |  //
                        ((uint64_t)buffer[25] << 48) |  //
                        ((uint64_t)buffer[24] << 56);   //
            chunks += "   " + string(buffer) + " - " +
                      std::to_string(chunksize) + "\n";
            binToStdout(&buffer[24], 8);

            if (strcmp(buffer, "desc") == 0) {
                this->file->read(buffer, 32);
                binToStdout(buffer, 32);

                sampleSize = (uint16_t)buffer[31];
                info +=
                    "sampleSize: " + std::to_string((int)sampleSize) + "bit\n";
                continue;
            }

            if (strcmp(buffer, "data") == 0) {
                readingData = true;
                bytesToRead = sampleSize / 8;
                this->file->read(buffer, 4);
                binToStdout(buffer, 4);  // skip mEditCount
                continue;
            }

            ulong i = 0;
            for (i = 0; i < chunksize; i++) {
                this->file->read(buffer, 1);
                binToStdout(buffer, 1);
            }
        } else {
            if (bvData->pop()) {
                buffer[1] |= 1;
            } else {
                buffer[1] &= ~1;
            }
            binToStdout(buffer, sampleSize == 16 ? 2 : 3);
        }
    }

    std::cerr << info;
    std::cerr << chunks;
}
