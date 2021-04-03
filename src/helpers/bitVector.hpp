#pragma once
#include <bits/stdc++.h>

#include <memory>
#include <ostream>
using std::bitset;
using std::make_unique;
using std::ostream;
using std::string;
using std::unique_ptr;

class bitVector {
   private:
    uint length = 0;
    unique_ptr<bitset<5000>> bits = make_unique<bitset<5000>>();

   public:
    void add(char c);
    void addFinal(char c);

    string toString() const;
    int getLength() const;
    const bitset<5000>& getBits() const;
};