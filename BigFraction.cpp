#include "BigFraction.h"

using namespace std;

BigFraction::BigFraction() {
    numerator = BigInteger(0);
    denominator = BigInteger(1);
}

BigFraction::BigFraction(const BigInteger& value) {
    numerator = value;
    denominator = BigInteger(1);
}

BigFraction::BigFraction(const BigInteger& numerator, const BigInteger& denominator) {
    this->numerator = numerator;
    this->denominator = denominator;
    reduce();
}

void BigFraction::reduce() {
    if (numerator.isZero()) {
        denominator = BigInteger(1);
        return;
    }

    BigInteger divisor = gcd(numerator, denominator);
    numerator = numerator / divisor;
    denominator = denominator / divisor;
}

bool BigFraction::isZero() const {
    return numerator.isZero();
}

string BigFraction::toString() const {
    if (denominator == BigInteger(1)) {
        return numerator.toString();
    }

    return numerator.toString() + "/" + denominator.toString();
}

BigInteger BigFraction::getNumerator() const {
    return numerator;
}

BigInteger BigFraction::getDenominator() const {
    return denominator;
}

BigInteger BigFraction::integerPart() const {
    return numerator / denominator;
}

BigInteger BigFraction::remainder() const {
    return numerator % denominator;
}

int BigFraction::compare(const BigFraction& other) const {
    BigInteger left = numerator * other.denominator;
    BigInteger right = other.numerator * denominator;

    return left.compare(right);
}

bool BigFraction::operator<(const BigFraction& other) const {
    return compare(other) < 0;
}

bool BigFraction::operator<=(const BigFraction& other) const {
    return compare(other) <= 0;
}

bool BigFraction::operator>(const BigFraction& other) const {
    return compare(other) > 0;
}

bool BigFraction::operator>=(const BigFraction& other) const {
    return compare(other) >= 0;
}

bool BigFraction::operator==(const BigFraction& other) const {
    return compare(other) == 0;
}

bool BigFraction::operator!=(const BigFraction& other) const {
    return compare(other) != 0;
}

BigFraction BigFraction::operator+(const BigFraction& other) const {
    BigInteger newNumerator = numerator * other.denominator + other.numerator * denominator;
    BigInteger newDenominator = denominator * other.denominator;

    return BigFraction(newNumerator, newDenominator);
}

BigFraction BigFraction::operator-(const BigFraction& other) const {
    BigInteger newNumerator = numerator * other.denominator - other.numerator * denominator;
    BigInteger newDenominator = denominator * other.denominator;

    return BigFraction(newNumerator, newDenominator);
}

BigFraction BigFraction::operator*(const BigFraction& other) const {
    BigInteger newNumerator = numerator * other.numerator;
    BigInteger newDenominator = denominator * other.denominator;

    return BigFraction(newNumerator, newDenominator);
}

BigFraction BigFraction::operator/(const BigFraction& other) const {
    BigInteger newNumerator = numerator * other.denominator;
    BigInteger newDenominator = denominator * other.numerator;

    return BigFraction(newNumerator, newDenominator);
}
