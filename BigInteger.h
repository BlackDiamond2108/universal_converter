#pragma once

#include <string>
#include <vector>
#include <utility>

class BigInteger {
private:
    static const int BASE = 1000000000;
    static const int BASE_DIGITS = 9;

    std::vector<int> digits;

    void trim();

    BigInteger multiply(const BigInteger& other) const;
    BigInteger sliceBlocks(std::size_t left, std::size_t right) const;
    BigInteger shiftBlocks(std::size_t blockCount) const;

    static BigInteger karatsubaMultiply(const BigInteger& x, const BigInteger& y);

    void multiplyByBaseAndAddBlock(int block);

public:
    BigInteger();
    BigInteger(long long value);
    BigInteger(const std::string& s);

    bool isZero() const;
    std::string toString() const;

    int compare(const BigInteger& other) const;

    bool operator<(const BigInteger& other) const;
    bool operator<=(const BigInteger& other) const;
    bool operator>(const BigInteger& other) const;
    bool operator>=(const BigInteger& other) const;
    bool operator==(const BigInteger& other) const;
    bool operator!=(const BigInteger& other) const;

    BigInteger operator+(const BigInteger& other) const;
    BigInteger operator-(const BigInteger& other) const;
    BigInteger operator*(const BigInteger& other) const;
    BigInteger operator/(const BigInteger& other) const;
    BigInteger operator%(const BigInteger& other) const;

    BigInteger mulInt(int m) const;

    std::pair<BigInteger, int> divModInt(int divisor) const;
    std::pair<BigInteger, BigInteger> divMod(const BigInteger& divisor) const;

    int toInt() const;
};

BigInteger gcd(BigInteger a, BigInteger b);