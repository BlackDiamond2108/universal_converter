#include "BaseConverter.h"

#include <algorithm>
#include <map>
#include <vector>

using namespace std;

namespace {

enum class TokenType {
    DIGIT,
    DOT,
    OPEN_PERIOD_BRACKET,
    CLOSE_PERIOD_BRACKET
};

struct DigitToken {
    TokenType type;
    int value;
    size_t position;
    string rawText;
};

bool isDecimalDigit(char c) {
    if (c >= '0' && c <= '9') {
        return true;
    }
    return false;
}

bool isLatinLetter(char c) {
    if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z')) {
        return true;
    }
    return false;
}

int letterToValue(char c) {
    if ('a' <= c && c <= 'z') {
        c = c - 'a' + 'A';
    }
    int value = c - 'A' + 10;
    return value;
}

int charDigitToValue(char c) {
    return (c - '0');
}

DigitToken readBracketDigit(const string& s, size_t& pos) {
    size_t start = pos;
    pos++;
    int value = 0;
    while(pos < s.size() && s[pos] != ']') {
        value = value * 10 + (s[pos] - '0');
        pos++;
    }
    pos++;
    DigitToken token;
    token.type = TokenType::DIGIT;
    token.value = value;
    token.position = start;
    token.rawText = s.substr(start, pos - start);

    return token;
}

string tokenTypeToString(TokenType type) { // for debug
    switch (type) {
        case TokenType::DIGIT:
            return "DIGIT";
        case TokenType::DOT:
            return "DOT";
        case TokenType::OPEN_PERIOD_BRACKET:
            return "OPEN_PERIOD_BRACKET";
        case TokenType::CLOSE_PERIOD_BRACKET:
            return "CLOSE_PERIOD_BRACKET";
    }

    return "UNKNOWN";
}

vector<DigitToken> tokenize(const string& s) {
    vector<DigitToken> tokens;
    size_t pos = 0;
    while (pos < s.size()) {
        char c = s[pos];
        if (isDecimalDigit(c)) {
            DigitToken token;
            token.type = TokenType::DIGIT;
            token.value = charDigitToValue(c);
            token.position = pos;
            token.rawText = string(1, c);
            tokens.push_back(token);
            pos++;
        } else if (isLatinLetter(c)) {
            DigitToken token;
            token.type = TokenType::DIGIT;
            token.value = letterToValue(c);
            token.position = pos;
            token.rawText = string(1, c);
            tokens.push_back(token);
            pos++;
        } else if(c == '[') {
            DigitToken token = readBracketDigit(s, pos);
            tokens.push_back(token);
        } else if (c == '.') {
            DigitToken token;
            token.type = TokenType::DOT;
            token.position = pos;
            token.rawText = string(1, c);
            pos++;
            tokens.push_back(token);
        } else if (c == '(') {
            DigitToken token;
            token.type = TokenType::OPEN_PERIOD_BRACKET;
            token.position = pos;
            token.rawText = string(1, c);
            pos++;
            tokens.push_back(token);
        } else if (c == ')') {
            DigitToken token;
            token.type = TokenType::CLOSE_PERIOD_BRACKET;
            token.position = pos;
            token.rawText = string(1, c);
            pos++;
            tokens.push_back(token);
        } else {
            pos++;
        }
    }
    return tokens;
}

enum class ParseState {
    INTEGER,
    FRACTION,
    PERIOD
};

struct ParsedNumber {
    vector<int> integerDigits;
    vector<int> fractionalDigits;
    vector<int> periodDigits;
    bool hasFractionalPart;
    bool hasPeriod;
};

ParsedNumber Parse(const vector<DigitToken>& tokens) {
    ParseState state = ParseState::INTEGER;
    ParsedNumber result;
    result.hasFractionalPart = false;
    result.hasPeriod = false;

    for (size_t i = 0; i < tokens.size(); i++) {
        DigitToken token = tokens[i];
        if (token.type == TokenType::DIGIT) {
            if (state == ParseState::INTEGER) {
                result.integerDigits.push_back(token.value);
            } else if (state == ParseState::FRACTION) {
                result.fractionalDigits.push_back(token.value);
            } else {
                result.periodDigits.push_back(token.value);
            }
        } else if (token.type == TokenType::DOT) {
            result.hasFractionalPart = true;
            state = ParseState::FRACTION;
        } else if (token.type == TokenType::OPEN_PERIOD_BRACKET) {
            result.hasFractionalPart = true;
            result.hasPeriod = true;
            state = ParseState::PERIOD;
        }
    }

    return result;
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

bool validateInputText(const string& input, int base, string& error) {
    if (input.empty()) {
        error = "Ошибка: пустое число";
        return false;
    }

    bool hasDot = false;
    bool inPeriod = false;
    bool hasPeriod = false;
    bool periodClosed = false;
    bool hasIntegerDigit = false;
    bool hasFractionalDigit = false;
    bool hasPeriodDigit = false;

    size_t pos = 0;
    while (pos < input.size()) {
        if (periodClosed) {
            error = "Ошибка в позиции " + positionText(pos) + ": после периода не должно быть символов";
            return false;
        }

        char c = input[pos];

        if (isDecimalDigit(c) || isLatinLetter(c)) {
            int value = isDecimalDigit(c) ? charDigitToValue(c) : letterToValue(c);
            if (value >= base) {
                error = "Ошибка в позиции " + positionText(pos) + ": цифра не подходит для основания " + to_string(base);
                return false;
            }

            if (!hasDot) {
                hasIntegerDigit = true;
            } else if (inPeriod) {
                hasPeriodDigit = true;
            } else {
                hasFractionalDigit = true;
            }

            pos++;
        } else if (c == '[') {
            size_t start = pos;
            pos++;

            if (pos >= input.size() || !isDecimalDigit(input[pos])) {
                error = "Ошибка в позиции " + positionText(start) + ": пустая цифра в квадратных скобках";
                return false;
            }

            int value = 0;
            while (pos < input.size() && input[pos] != ']') {
                if (!isDecimalDigit(input[pos])) {
                    error = "Ошибка в позиции " + positionText(pos) + ": внутри квадратных скобок должны быть только цифры";
                    return false;
                }

                if (value <= 1000000) {
                    value = value * 10 + charDigitToValue(input[pos]);
                }
                pos++;
            }

            if (pos >= input.size()) {
                error = "Ошибка в позиции " + positionText(start) + ": не закрыта квадратная скобка";
                return false;
            }

            if (value >= base) {
                error = "Ошибка в позиции " + positionText(start) + ": цифра не подходит для основания " + to_string(base);
                return false;
            }

            if (!hasDot) {
                hasIntegerDigit = true;
            } else if (inPeriod) {
                hasPeriodDigit = true;
            } else {
                hasFractionalDigit = true;
            }

            pos++;
        } else if (c == '.') {
            if (hasDot) {
                error = "Ошибка в позиции " + positionText(pos) + ": лишняя точка";
                return false;
            }

            if (!hasIntegerDigit) {
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

            if (!hasPeriodDigit) {
                error = "Ошибка в позиции " + positionText(pos) + ": период не может быть пустым";
                return false;
            }

            inPeriod = false;
            periodClosed = true;
            pos++;
        } else {
            error = "Ошибка в позиции " + positionText(pos) + ": недопустимый символ";
            return false;
        }
    }

    if (!hasIntegerDigit) {
        error = "Ошибка: отсутствует целая часть";
        return false;
    }

    if (inPeriod) {
        error = "Ошибка: не закрыта скобка периода";
        return false;
    }

    if (hasDot && !hasFractionalDigit && !hasPeriodDigit) {
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
            value = value * 10 + charDigitToValue(c);
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
    if (!validateInputText(input, base, error)) {
        return BigFraction();
    }

    vector<DigitToken> tokens = tokenize(input);
    ParsedNumber parsed = Parse(tokens);

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
