#include <iostream>

#include "fileParsers/wavParser.hpp"
#include "helpers/optionParser.hpp"

int main(int argc, char** argv) {
    // hexdump -C ./data/de-oude-schicht.wav | less
    const info info = optionParser::getInfo(argc, argv);

    wavParser wp{};
    if (info._mode == mode::decode) {
        string str = wp.getData(info._file);
        std::cout << str << std::endl;
    } else {
        wp.setData(info._file, info._text);
    }
}