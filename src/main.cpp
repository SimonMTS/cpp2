#include <iostream>
#include <memory>

#include "fileParsers/cafParser.hpp"
#include "fileParsers/fileParser.hpp"
#include "fileParsers/wavParser.hpp"
#include "helpers/optionParser.hpp"
using std::make_unique;

int main(int argc, char** argv) {
    const info info = optionParser::getInfo(argc, argv);

    unique_ptr<fileParser> fp;

    const string last3chars = info._file.substr(info._file.size() - 3, 3);
    if (last3chars == "wav") {
        fp = make_unique<wavParser>(info._file);
    } else if (last3chars == "caf") {
        fp = make_unique<cafParser>(info._file);
    } else if (last3chars == "aif") {
        // fp = make_unique<aifParser>(info._file);
        std::cout << "Unsupported file format\n";
        exit(1);
    } else {
        std::cout << "Unsupported file format\n";
        exit(1);
    }

    if (info._mode == mode::decode) {
        fp->getData();
    } else {
        fp->setData(info._text);
    }
}
