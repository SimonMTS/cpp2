#pragma once
#include <iostream>
#include <string>
using std::string;

class fileParser {
   protected:
    static void binToStdout(const char* const data, int length) {
        for (int i = 0; i < length; i++) {
            std::cout << data[i];
        }
    }

   public:
    virtual void getData() const = 0;
    virtual void setData(const string& data) const = 0;
};
