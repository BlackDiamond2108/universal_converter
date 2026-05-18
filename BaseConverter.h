#pragma once

#include <cstddef>
#include <string>

#include "BigFraction.h"
#include "BigInteger.h"

const std::size_t MAX_OUTPUT_LENGTH = 1000000;

struct ConversionResult {
    bool success;
    bool truncated;
    std::string output;
    std::string message;
};

class BaseConverter {
private:
    bool parseBase(const std::string& text, const std::string& name, int& base, std::string& error) const;
    BigFraction parseInput(const std::string& input, int base, std::string& error) const;
    std::string formatFraction(const BigFraction& fraction, int base, bool& truncated) const;

public:
    ConversionResult convert(const std::string& input, const std::string& sourceBase, const std::string& targetBase) const;
};
