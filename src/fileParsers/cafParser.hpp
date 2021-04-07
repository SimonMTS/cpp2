#pragma once
#include <fstream>
#include <memory>
#include <string>

#include "fileParser.hpp"
using std::ifstream;
using std::string;
using std::unique_ptr;

class cafParser : public fileParser {
   private:
    unique_ptr<ifstream> file;

   public:
    cafParser(const string& filename);
    void getData() const override;
    void setData(const string& data) const override;
};
