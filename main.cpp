#include <iostream>

using namespace std;

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

    int readBracketDigit
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