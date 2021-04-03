#include "optionParser.hpp"

#include <getopt.h>

#include <iostream>

bool info::isValid() const {
    if (_mode == mode::none) return false;
    if (_mode == mode::encode && (_text == "" || _file == "")) return false;
    if (_mode == mode::decode && _file == "") return false;
    return true;
}

void optionParser::printUsage() {
    std::cout << "\n"
                 "-d, --decode:  Get hidden text from file, "
                 "and print to stdout.\n"
                 "-e, --encode:  Hide text in file\n"
                 "-h, --help:    Show this message.\n\n"
                 "example encode:\n"
                 "\taSteg -e 'text' -f ./input.wav > ./output.wav\n"
                 "example decode:\n"
                 "\taSteg -d -f ./input.wav\n\n";
    exit(1);
}

info optionParser::getInfo(int argc, char** argv) {
    info i;

    const char* const short_opts = "de:f:h";
    const option long_opts[] = {{"encode", required_argument, nullptr, 'e'},
                                {"decode", required_argument, nullptr, 'd'},
                                {"help", no_argument, nullptr, 'h'}};

    while (true) {
        const auto opt =
            getopt_long(argc, argv, short_opts, long_opts, nullptr);
        if (opt == -1) break;

        switch (opt) {
            case 'd':
                i._mode = mode::decode;
                break;

            case 'e':
                i._mode = mode::encode;
                i._text = optarg;
                break;

            case 'f':
                i._file = optarg;
                break;

            case 'h':
            case '?':
            default:
                optionParser::printUsage();
                break;
        }
    }

    if (!i.isValid()) {
        optionParser::printUsage();
    }

    return i;
}