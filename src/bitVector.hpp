#pragma once
#include <bits/stdc++.h>

#include <memory>
#include <ostream>
using std::bitset;
using std::ostream;
using std::unique_ptr;

class bitVector {
   private:
   public:
    uint length = 0;                                                  // bad
    unique_ptr<bitset<5000>> val = std::make_unique<bitset<5000>>();  // bad

    void addFinal(char c);
};

ostream& operator<<(ostream& str, bitVector& bv);