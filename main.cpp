#include <iostream>
#include <string>

#include "BaseConverter.h"

using namespace std;

int main() {
    string p;
    string q;
    string input;

    cin >> p >> q >> input;

    BaseConverter converter;
    ConversionResult result = converter.convert(input, p, q);

    if (!result.success) {
        cout << result.message << endl;
        return 0;
    }

    cout << result.output << endl;

    if (!result.message.empty()) {
        cout << result.message << endl;
    }

    return 0;
}
