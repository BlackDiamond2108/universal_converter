#pragma once

#include <string>

#include "BigInteger.h"

class BigFraction {
private:
    BigInteger numerator;
    BigInteger denominator;

    void reduce();

public:
    BigFraction();
    BigFraction(const BigInteger& value);
    BigFraction(const BigInteger& numerator, const BigInteger& denominator);

    bool isZero() const;
    std::string toString() const;

    BigInteger getNumerator() const;
    BigInteger getDenominator() const;

    BigInteger integerPart() const;
    BigInteger remainder() const;

    int compare(const BigFraction& other) const;

    bool operator<(const BigFraction& other) const;
    bool operator<=(const BigFraction& other) const;
    bool operator>(const BigFraction& other) const;
    bool operator>=(const BigFraction& other) const;
    bool operator==(const BigFraction& other) const;
    bool operator!=(const BigFraction& other) const;

    BigFraction operator+(const BigFraction& other) const;
    BigFraction operator-(const BigFraction& other) const;
    BigFraction operator*(const BigFraction& other) const;
    BigFraction operator/(const BigFraction& other) const;
};
