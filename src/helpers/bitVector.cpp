#include "bitVector.hpp"

#include <array>

string bitVector::toString() const {
    std::array<char, 5000> res{};
    for (uint i = 0; i < this->length; i++) {
        res[(i - (i % 8)) / 8] <<= 1;
        res[(i - (i % 8)) / 8] |= (*this->bits)[i];
    }
    res[4999] = '\0';

    return res.data();
}

void bitVector::add(char c) {
    //
}

void bitVector::addFinal(char c) {
    if (length >= 4999) return;

    c &= 0x1;
    bool asd = c == 1;
    this->bits->set(this->length, asd);
    this->length++;
}

int bitVector::getLength() const { return this->length; }

const bitset<5000>& bitVector::getBits() const { return *this->bits; }