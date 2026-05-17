#include "BigInteger.h"

#include <algorithm>
#include <stdexcept>

using namespace std;

BigInteger::BigInteger() {

}

BigInteger::BigInteger(long long value) {
    while (value > 0) {
        digits.push_back(value % BASE); 
        value /= BASE;
    }
}

BigInteger::BigInteger(const std::string& s) {
    for (int end = s.size(); end > 0; end -= BASE_DIGITS) {
        int start = max(0, end - BASE_DIGITS);
        int length = end - start;

        int block = 0;

        for (int i = start; i < end; i++) {
            block = block * 10 + (s[i] - '0');
        }
        digits.push_back(block);
    }

    trim();
}

void BigInteger::trim() {
    while(!digits.empty() && digits.back() == 0) {
        digits.pop_back();
    }
}

bool BigInteger::isZero() const {
    return digits.empty();
}

string BigInteger::toString() const {
    
}