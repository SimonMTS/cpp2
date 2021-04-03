#pragma once
#include <string>
using std::string;
#include "fileParser.hpp"

class wavParser : public fileParser {
   public:
    string getData(const string& filename) override;
    void setData(const string& filename, const string& data) override;
};