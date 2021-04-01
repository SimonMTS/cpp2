#include <bits/stdc++.h>
#include <stdio.h>
#include <string.h>

#include <fstream>
#include <iostream>
#include <vector>

int main() {
    // hexdump -C ./data/de-oude-schicht.wav | less
    // std::cout << "Hello World!" << std::endl;

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

        std::vector<char> data{};
        // std::bitset<0> bset1; // use to make bit vector?
        if (strcmp(chunkName, "data") == 0) {
            char sampleRaw[3];
            while (myFile.read(sampleRaw, 2)) {
                sampleRaw[2] = '\0';

                uint16_t sample1 = 0x00;
                uint16_t sample2 = 0x00;
                uint16_t sample3 = 0x00;
                sample1 |= (uint8_t)sampleRaw[1];
                sample2 |= (uint8_t)sampleRaw[0];

                sample3 = 0x0000;
                sample3 |= sample1;
                sample3 <<= 8;
                sample3 |= sample2;

                data.push_back(sampleRaw[0]);

                // char asd[2];
                // asd[0] = sampleRaw[1];
                // asd[1] = sampleRaw[0];
                // float qwe;
                // memcpy(sampleRaw, &qwe, 2);

                // std::cout << qwe << std::endl;
            }
        }

        char res[5000];
        for (int i = 0; i < 5000; i++) {
            res[i] = 0x00;

            if (data.size() * 8 > i) {
                for (int j = i * 8; j < (i * 8) + 8; j++) {
                    char tmp = data[j];
                    tmp &= 0x1;

                    res[i] <<= 1;
                    res[i] |= tmp;
                }
            }
        }

        res[4999] = '\0';
        std::cout << res << std::endl;

        // if chunkname is 'fmt ' or 'data' process it here,
        // otherwise skip any unknown chunk:
        myFile.seekg(chunksize, std::ios_base::cur);
    }
}