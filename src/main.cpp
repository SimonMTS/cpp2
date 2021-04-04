#include <iostream>

#include "fileParsers/wavParser.hpp"
#include "helpers/optionParser.hpp"

int main(int argc, char** argv) {
    // hexdump -C ./data/de-oude-schicht.wav | less
    const info info = optionParser::getInfo(argc, argv);

    wavParser wp{info._file};
    if (info._mode == mode::decode) {
        wp.getData();
    } else {
        wp.setData(info._text);
    }
}