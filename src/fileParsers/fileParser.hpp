#pragma once
#include <string>
using std::string;

class fileParser {
   public:
    virtual string getData(const string& filename) = 0;
    virtual void setData(const string& filename, const string& data) = 0;
};