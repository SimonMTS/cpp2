#pragma once
#include <string>

using std::string;

enum mode { encode, decode, none };

class info {
   public:
    mode _mode;
    string _text;
    string _file;

    info() : _mode(mode::none), _text(""), _file("") {}

    bool isValid() const;
};

class optionParser {
   private:
    static void printUsage();

   public:
    // getopt_long expects exactly these arguments
    static info getInfo(int argc, char** argv);
};