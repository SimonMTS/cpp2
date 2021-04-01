#include <stdio.h>
#include <string.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "bitVector.hpp"

int main() {
    // hexdump -C ./data/de-oude-schicht.wav | less

    std::unique_ptr<std::ifstream> myFile = std::make_unique<std::ifstream>(
        "./data/de-oude-schicht.wav", std::ios::in | std::ios::binary);
    char buffer[12];
    myFile->read(buffer, 12);  // skip RIFF header

    char chunkName[5];
    unsigned long chunksize;
    while (myFile->read(chunkName, 4)) {
        chunkName[4] = '\0';
        myFile->read((char*)&chunksize, 4);
        std::cout << chunkName << " - " << chunksize << std::endl;

        if (strcmp(chunkName, "data") == 0) {
            std::unique_ptr<bitVector> bvData = std::make_unique<bitVector>();
            char sampleRaw[2];
            while (myFile->read(sampleRaw, 2)) {
                bvData->addFinal(sampleRaw[0]);
            }
            std::cout << "secret:\n" << *bvData << std::endl;
        }

        myFile->seekg(chunksize, std::ios_base::cur);
    }
}