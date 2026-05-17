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
    if (isZero()) {
        return "0";
    }
    string result = to_string(digits.back());
    for (int i = digits.size() - 2; i >= 0; i--) {
        string block = to_string(digits[i]);
    while(block.size() < BASE_DIGITS) {
        block = "0" + block;
    }

    result += block;
    }
    return result;
}

int BigInteger::compare(const BigInteger& other) const {
    if (digits.size() < other.digits.size()) {
        return -1;
    }
    if (digits.size() > other.digits.size()) {
        return 1;
    }
    for (int i = digits.size() - 1; i >= 0; i--) {
        if (digits[i] < other.digits[i]) {
            return -1;
        }

        if (digits[i] > other.digits[i]) {
            return 1;
        }
    }

    return 0;
}

bool BigInteger::operator<(const BigInteger& other) const {
    return compare(other) < 0;
}

bool BigInteger::operator<=(const BigInteger& other) const {
    return compare(other) <= 0;
}

bool BigInteger::operator>(const BigInteger& other) const {
   return compare(other) > 0;
}

bool BigInteger::operator>=(const BigInteger& other) const {
    return compare(other) >= 0;
}

bool BigInteger::operator==(const BigInteger& other) const {
    return compare(other) == 0;
}

bool BigInteger::operator!=(const BigInteger& other) const {
    return compare(other) != 0;
}

BigInteger BigInteger::operator+(const BigInteger& other) const {
    BigInteger result;

    int n = max(digits.size(), other.digits.size());
    long long carry = 0;

    for (int i = 0; i < n; i++) {
        long long current = carry;
        if (i < digits.size()) {
            current += digits[i];
        }

        if (i < other.digits.size()) {
            current += other.digits[i];
        }

        result.digits.push_back(current%BASE);
        carry = current / BASE;
    }
    if (carry > 0) {
        result.digits.push_back(carry);
    }

    result.trim();
    return result;
}

BigInteger BigInteger::operator-(const BigInteger& other) const {
    BigInteger result;

    long long borrow = 0;

    for (int i = 0; i < (int)digits.size(); i++) {
        long long current = digits[i] - borrow;

        if (i < (int)other.digits.size()) {
            current -= other.digits[i];
        }

        if (current < 0) {
            current += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }

        result.digits.push_back(current);
    }

    result.trim();
    return result;
}

BigInteger BigInteger::mulInt(int m) const {

    if (isZero() || m == 0) {
        return BigInteger(0);
    }

    BigInteger result;
    long long carry = 0;

    for (int i = 0; i < (int)digits.size(); i++) {
        long long current = 1LL * digits[i] * m + carry;

        result.digits.push_back(current % BASE);
        carry = current / BASE;
    }

    while (carry > 0) {
        result.digits.push_back(carry % BASE);
        carry /= BASE;
    }

    result.trim();
    return result;
}

BigInteger operator*(const BigInteger& a, int m) {
    return a.mulInt(m);
}

BigInteger operator*(int m, const BigInteger& a) {
    return a.mulInt(m);
}

pair<BigInteger, int> BigInteger::divModInt(int divisor) const {

    BigInteger quotient;
    quotient.digits.resize(digits.size());

    long long remainder = 0;

    for (int i = (int)digits.size() - 1; i >= 0; i--) {
        long long current = remainder * BASE + digits[i];

        quotient.digits[i] = current / divisor;
        remainder = current % divisor;
    }

    quotient.trim();

    return {quotient, (int)remainder};
}

BigInteger BigInteger::mulBig(const BigInteger& other) const {
    if (isZero() || other.isZero()) {
        return BigInteger(0);
    }

    BigInteger result;
    result.digits.assign(digits.size() + other.digits.size() + 1, 0);

    for (int i = 0; i < (int)digits.size(); i++) {
        long long carry = 0;

        for (int j = 0; j < (int)other.digits.size() || carry > 0; j++) {
            long long current = result.digits[i + j] + carry;

            if (j < (int)other.digits.size()) {
                current += 1LL * digits[i] * other.digits[j];
            }

            result.digits[i + j] = current % BASE;
            carry = current / BASE;
        }
    }

    result.trim();
    return result;
}

BigInteger BigInteger::sliceBlocks(size_t left, size_t right) const {
    BigInteger result;

    for (size_t i = left; i < right && i < digits.size(); i++) {
        result.digits.push_back(digits[i]);
    }

    result.trim();
    return result;
}

BigInteger BigInteger::shiftBlocks(size_t blockCount) const {
    if (isZero()) {
        return BigInteger(0);
    }

    BigInteger result;

    for (size_t i = 0; i < blockCount; i++) {
        result.digits.push_back(0);
    }

    for (size_t i = 0; i < digits.size(); i++) {
        result.digits.push_back(digits[i]);
    }

    return result;
}

BigInteger BigInteger::karatsubaMultiply(const BigInteger& x, const BigInteger& y) {
    if (x.isZero() || y.isZero()) {
        return BigInteger(0);
    }

    size_t n = max(x.digits.size(), y.digits.size());

    if (n <= 32) {
        return x.mulBig(y);
    }

    size_t k = n / 2;

    BigInteger x0 = x.sliceBlocks(0, k);
    BigInteger x1 = x.sliceBlocks(k, x.digits.size());

    BigInteger y0 = y.sliceBlocks(0, k);
    BigInteger y1 = y.sliceBlocks(k, y.digits.size());

    BigInteger z0 = karatsubaMultiply(x0, y0);
    BigInteger z2 = karatsubaMultiply(x1, y1);

    BigInteger sumX = x0 + x1;
    BigInteger sumY = y0 + y1;

    BigInteger z1 = karatsubaMultiply(sumX, sumY);

    z1 = z1 - z0;
    z1 = z1 - z2;

    BigInteger result = z2.shiftBlocks(2 * k) + z1.shiftBlocks(k) + z0;

    result.trim();
    return result;
}

BigInteger BigInteger::operator*(const BigInteger& other) const {
    return karatsubaMultiply(*this, other);
}

void BigInteger::multiplyByBaseAndAddBlock(int block) {

    if (isZero()) {
        if (block != 0) {
            digits.push_back(block);
        }
        return;
    }

    digits.insert(digits.begin(), block);
    trim();
}

pair<BigInteger, BigInteger> BigInteger::divMod(const BigInteger& divisor) const {


    if (*this < divisor) {
        return {BigInteger(0), *this};
    }

    BigInteger quotient;
    BigInteger remainder;

    quotient.digits.assign(digits.size(), 0);

    for (int i = (int)digits.size() - 1; i >= 0; i--) {
        remainder.multiplyByBaseAndAddBlock(digits[i]);

        int left = 0;
        int right = BASE - 1;
        int best = 0;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            BigInteger product = divisor.mulInt(mid);

            if (product <= remainder) {
                best = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }

        quotient.digits[i] = best;

        BigInteger subtractValue = divisor.mulInt(best);
        remainder = remainder - subtractValue;
    }

    quotient.trim();
    remainder.trim();

    return {quotient, remainder};
}

BigInteger BigInteger::operator/(const BigInteger& other) const {
    return BigInteger::divMod(other).first;
}

BigInteger BigInteger::operator%(const BigInteger& other) const {
    return BigInteger::divMod(other).second;
}

int BigInteger::toInt() const {
    long long result = 0;

    for (int i = (int)digits.size() - 1; i >= 0; i--) {
        result = result * BASE + digits[i];
    }

    return (int)result;
};

BigInteger gcd(BigInteger a, BigInteger b) {
    while (!b.isZero()) {
        BigInteger remainder = a % b;
        a = b;
        b = remainder;
    }

    return a;
}
