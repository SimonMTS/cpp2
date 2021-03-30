#include <stdio.h>
#include <string.h>

#include <fstream>
#include <iostream>

int main() {
    std::cout << "Hello World!" << std::endl;

    std::ifstream myFile("./data/de-oude-schicht.wav",
                         std::ios::in | std::ios::binary);
    char buffer[12];
    myFile.read(buffer, 12);  // skip RIFF header

    char chunkName[5];
    unsigned long chunksize;
    while (myFile.read(chunkName, 4)) {
        chunkName[4] = '\0';  // add trailing zero
        myFile.read((char*)&chunksize, 4);

        std::cout << chunkName << " - " << chunksize << std::endl;

        if (strcmp(chunkName, "data") == 0) {
            char sampleRaw[3];
            uint16_t sample;
            while (myFile.read(sampleRaw, 2)) {
                chunkName[3] = '\0';  // add trailing zero
                sample = (uint16_t)*sampleRaw;

                std::cout << sample << std::endl;
            }
        }

        // if chunkname is 'fmt ' or 'data' process it here,
        // otherwise skip any unknown chunk:
        myFile.seekg(chunksize, std::ios_base::cur);
    }
}