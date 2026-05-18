#include "BaseConverter.h"

#include <algorithm>
#include <map>
#include <vector>

using namespace std;

namespace {

struct ParsedInput {
    vector<int> integerDigits;
    vector<int> fractionalDigits;
    vector<int> periodDigits;
};

bool isDecimalDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isLatinLetter(char c) {
    return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
}

int digitValue(char c) {
    if (isDecimalDigit(c)) {
        return c - '0';
    }

    if ('a' <= c && c <= 'z') {
        c = c - 'a' + 'A';
    }

    return c - 'A' + 10;
}

string digitToString(int digit) {
    if (digit < 10) {
        return string(1, char('0' + digit));
    }

    if (digit < 36) {
        return string(1, char('A' + digit - 10));
    }

    return "[" + to_string(digit) + "]";
}

BigInteger powerInt(int base, size_t exponent) {
    BigInteger result(1);

    for (size_t i = 0; i < exponent; i++) {
        result = result.mulInt(base);
    }

    return result;
}

BigInteger digitsToBigInteger(const vector<int>& digits, int base) {
    BigInteger result(0);

    for (int digit : digits) {
        result = result.mulInt(base) + BigInteger(digit);
    }

    return result;
}

string positionText(size_t position) {
    return to_string(position + 1);
}

bool appendWithLimit(string& result, const string& part, bool& truncated) {
    if (truncated) {
        return false;
    }

    result += part;

    if (result.size() > MAX_OUTPUT_LENGTH) {
        result = result.substr(0, MAX_OUTPUT_LENGTH);
        truncated = true;
        return false;
    }

    return true;
}

string formatIntegerPart(BigInteger value, int base, bool& truncated) {
    if (value.isZero()) {
        return "0";
    }

    vector<int> digits;

    while (!value.isZero()) {
        pair<BigInteger, int> divMod = value.divModInt(base);
        value = divMod.first;
        digits.push_back(divMod.second);
    }

    reverse(digits.begin(), digits.end());

    string result;
    for (int digit : digits) {
        if (!appendWithLimit(result, digitToString(digit), truncated)) {
            break;
        }
    }

    return result;
}

void appendFractionDigitsWithLimit(string& result, const vector<string>& digits, bool& truncated) {
    for (const string& digit : digits) {
        if (!appendWithLimit(result, digit, truncated)) {
            return;
        }
    }
}

bool readBracketDigit(const string& input, size_t& pos, int base, int& value, string& error) {
    size_t start = pos;
    pos++;

    if (pos >= input.size() || !isDecimalDigit(input[pos])) {
        error = "Ошибка в позиции " + positionText(start) + ": пустая цифра в квадратных скобках";
        return false;
    }

    long long current = 0;

    while (pos < input.size() && input[pos] != ']') {
        if (!isDecimalDigit(input[pos])) {
            error = "Ошибка в позиции " + positionText(pos) + ": внутри квадратных скобок должны быть только цифры";
            return false;
        }

        if (current <= 1000000) {
            current = current * 10 + (input[pos] - '0');
        }
        pos++;
    }

    if (pos >= input.size()) {
        error = "Ошибка в позиции " + positionText(start) + ": не закрыта квадратная скобка";
        return false;
    }

    if (current >= base) {
        error = "Ошибка в позиции " + positionText(start) + ": цифра не подходит для основания " + to_string(base);
        return false;
    }

    value = (int)current;
    pos++;
    return true;
}

bool parseNumberText(const string& input, int base, ParsedInput& result, string& error) {
    if (input.empty()) {
        error = "Ошибка: пустое число";
        return false;
    }

    bool hasDot = false;
    bool inPeriod = false;
    bool hasPeriod = false;
    bool periodClosed = false;

    size_t pos = 0;

    while (pos < input.size()) {
        if (periodClosed) {
            error = "Ошибка в позиции " + positionText(pos) + ": после периода не должно быть символов";
            return false;
        }

        char c = input[pos];

        if (c == '.') {
            if (hasDot) {
                error = "Ошибка в позиции " + positionText(pos) + ": лишняя точка";
                return false;
            }

            if (result.integerDigits.empty()) {
                error = "Ошибка в позиции " + positionText(pos) + ": перед точкой должна быть целая часть";
                return false;
            }

            hasDot = true;
            pos++;
        } else if (c == '(') {
            if (!hasDot) {
                error = "Ошибка в позиции " + positionText(pos) + ": период может быть только после точки";
                return false;
            }

            if (inPeriod || hasPeriod) {
                error = "Ошибка в позиции " + positionText(pos) + ": лишняя открывающая скобка периода";
                return false;
            }

            inPeriod = true;
            hasPeriod = true;
            pos++;
        } else if (c == ')') {
            if (!inPeriod) {
                error = "Ошибка в позиции " + positionText(pos) + ": лишняя закрывающая скобка периода";
                return false;
            }

            if (result.periodDigits.empty()) {
                error = "Ошибка в позиции " + positionText(pos) + ": период не может быть пустым";
                return false;
            }

            inPeriod = false;
            periodClosed = true;
            pos++;
        } else {
            int value = 0;

            if (isDecimalDigit(c) || isLatinLetter(c)) {
                value = digitValue(c);

                if (value >= base) {
                    error = "Ошибка в позиции " + positionText(pos) + ": цифра не подходит для основания " + to_string(base);
                    return false;
                }

                pos++;
            } else if (c == '[') {
                if (!readBracketDigit(input, pos, base, value, error)) {
                    return false;
                }
            } else {
                error = "Ошибка в позиции " + positionText(pos) + ": недопустимый символ";
                return false;
            }

            if (!hasDot) {
                result.integerDigits.push_back(value);
            } else if (inPeriod) {
                result.periodDigits.push_back(value);
            } else {
                result.fractionalDigits.push_back(value);
            }
        }
    }

    if (result.integerDigits.empty()) {
        error = "Ошибка: отсутствует целая часть";
        return false;
    }

    if (inPeriod) {
        error = "Ошибка: не закрыта скобка периода";
        return false;
    }

    if (hasDot && result.fractionalDigits.empty() && result.periodDigits.empty()) {
        error = "Ошибка: после точки должна быть дробная часть или период";
        return false;
    }

    return true;
}

}

bool BaseConverter::parseBase(const string& text, const string& name, int& base, string& error) const {
    if (text.empty()) {
        error = "Ошибка: пустое основание " + name;
        return false;
    }

    int value = 0;

    for (char c : text) {
        if (!isDecimalDigit(c)) {
            error = "Ошибка: основание " + name + " должно быть целым числом";
            return false;
        }

        if (value <= 1000000) {
            value = value * 10 + (c - '0');
        }
    }

    if (value < 2 || value > 500) {
        error = "Ошибка: основание " + name + " должно быть в диапазоне от 2 до 500";
        return false;
    }

    base = value;
    return true;
}

BigFraction BaseConverter::parseInput(const string& input, int base, string& error) const {
    ParsedInput parsed;

    if (!parseNumberText(input, base, parsed, error)) {
        return BigFraction();
    }

    BigInteger integerPart = digitsToBigInteger(parsed.integerDigits, base);
    BigInteger fractionalPart = digitsToBigInteger(parsed.fractionalDigits, base);
    BigInteger basePowerFraction = powerInt(base, parsed.fractionalDigits.size());

    if (parsed.periodDigits.empty()) {
        BigInteger numerator = integerPart * basePowerFraction + fractionalPart;
        BigInteger denominator = basePowerFraction;

        return BigFraction(numerator, denominator);
    }

    BigInteger periodPart = digitsToBigInteger(parsed.periodDigits, base);
    BigInteger basePowerPeriod = powerInt(base, parsed.periodDigits.size());
    BigInteger periodDenominator = basePowerPeriod - BigInteger(1);

    BigInteger denominator = basePowerFraction * periodDenominator;
    BigInteger numerator = integerPart * denominator + fractionalPart * periodDenominator + periodPart;

    return BigFraction(numerator, denominator);
}

string BaseConverter::formatFraction(const BigFraction& fraction, int base, bool& truncated) const {
    truncated = false;

    string result = formatIntegerPart(fraction.integerPart(), base, truncated);

    if (truncated) {
        return result;
    }

    BigInteger remainder = fraction.remainder();

    if (remainder.isZero()) {
        return result;
    }

    appendWithLimit(result, ".", truncated);

    if (truncated) {
        return result;
    }

    BigInteger denominator = fraction.getDenominator();
    map<BigInteger, size_t> remainderPositions;
    vector<string> fractionalDigits;
    size_t fractionalLength = 0;
    size_t periodStart = fractionalDigits.size();
    bool hasPeriod = false;

    while (!remainder.isZero()) {
        if (remainderPositions.find(remainder) != remainderPositions.end()) {
            periodStart = remainderPositions[remainder];
            hasPeriod = true;
            break;
        }

        remainderPositions[remainder] = fractionalDigits.size();

        remainder = remainder.mulInt(base);

        BigInteger digit = remainder / denominator;
        remainder = remainder % denominator;

        string digitText = digitToString(digit.toInt());
        fractionalDigits.push_back(digitText);
        fractionalLength += digitText.size();

        if (result.size() + fractionalLength > MAX_OUTPUT_LENGTH) {
            appendFractionDigitsWithLimit(result, fractionalDigits, truncated);
            return result;
        }
    }

    for (size_t i = 0; i < fractionalDigits.size(); i++) {
        if (hasPeriod && i == periodStart) {
            appendWithLimit(result, "(", truncated);
        }

        if (!appendWithLimit(result, fractionalDigits[i], truncated)) {
            return result;
        }
    }

    if (hasPeriod) {
        appendWithLimit(result, ")", truncated);
    }

    return result;
}

ConversionResult BaseConverter::convert(const string& input, const string& sourceBase, const string& targetBase) const {
    ConversionResult result;
    result.success = false;
    result.truncated = false;

    int p = 0;
    int q = 0;

    if (!parseBase(sourceBase, "p", p, result.message)) {
        return result;
    }

    if (!parseBase(targetBase, "q", q, result.message)) {
        return result;
    }

    string error;
    BigFraction fraction = parseInput(input, p, error);

    if (!error.empty()) {
        result.message = error;
        return result;
    }

    result.output = formatFraction(fraction, q, result.truncated);
    result.success = true;

    if (result.truncated) {
        result.message = "Результат слишком длинный, показаны первые " + to_string(MAX_OUTPUT_LENGTH) + " символов";
    }

    return result;
}
