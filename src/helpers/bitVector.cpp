#include "bitVector.hpp"

bitVector::bitVector(const string& str) {
    // for (const char c : str) {
    //     this->push(c);
    // }

    for (int i = str.length() - 1; i >= 0; i--) {
        this->push(str[i]);
    }
}

string bitVector::toString() const {
    vector<char> res{};
    for (uint i = 0; i < this->length; i++) {
        ulong index = (i - (i % 8)) / 8;
        if (index <= res.size()) res.push_back('\0');

        res[index] <<= 1;
        res[index] |= this->data[i];
    }
    res.push_back('\0');

    return res.data();
}

void bitVector::push(char c) {
    for (int i = 0; i < 8; i++) {
        this->pushFinal(c);
        c >>= 1;
    }
}

void bitVector::pushFinal(char c) {
    c &= 0x1;
    this->data.push_back(c == 1);
    this->length++;
}

bool bitVector::pop() {
    bool res = this->data.back();
    this->data.pop_back();
    return res;
}

int bitVector::getLength() const { return this->length; }