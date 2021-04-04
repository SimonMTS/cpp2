#pragma once
#include <string>
using std::string;

class fileParser {
   public:
    virtual void getData() const = 0;
    virtual void setData(const string& data) const = 0;
};