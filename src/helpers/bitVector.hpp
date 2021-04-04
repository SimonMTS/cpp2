#pragma once
#include <ostream>
#include <vector>
using std::ostream;
using std::string;
using std::vector;

class bitVector {
   private:
    uint length = 0;
    vector<bool> data{};

   public:
    bitVector(){};
    bitVector(const string& str);

    void push(char c);
    void pushFinal(char c);

    bool pop();

    string toString() const;
    int getLength() const;
};