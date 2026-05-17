#include <iostream>
#include <vector>
#include <string>
#include "BigInteger.h"

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
            pos++;
            tokens.push_back(token);
        } else if (c == '(') {
            DigitToken token;
            token.type = TokenType::OPEN_PERIOD_BRACKET;
            token.position = pos;
            pos++;
            tokens.push_back(token);
        } else if (c == ')') {
            DigitToken token;
            token.type = TokenType::CLOSE_PERIOD_BRACKET;
            token.position = pos;
            pos++;
            tokens.push_back(token);
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
    for (int i = 0; i <= tokens.size() - 1; i++) {
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
            state = ParseState::PERIOD;
        }
    }
}

int main() {
    string s;
    cin >> s;

    vector<DigitToken> tokens = tokenize(s);

    for (DigitToken token : tokens) {
        cout << "Type = " << tokenTypeToString(token.type)
             << ", value = " << token.value
             << ", position = " << token.position
             << endl;
    }

    return 0;
}