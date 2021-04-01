#include "bitVector.hpp"

#include <array>

void bitVector::addFinal(char c) {
    if (length >= 4999) return;

    c &= 0x1;
    bool asd = c == 1;
    this->val->set(this->length, asd);
    this->length++;
}

ostream& operator<<(ostream& str, bitVector& bv) {
    if (bv.length == 0) return str;

    std::array<char, 5000> res{};
    for (int i = 0; i < bv.length; i++) {
        res[(i - (i % 8)) / 8] <<= 1;
        res[(i - (i % 8)) / 8] |= (*bv.val)[i];
    }
    res[4999] = '\0';

    return str << res.data();
}