#include <concepts>
#include <iostream>
#include <span>
#include <vector>

std::vector<std::byte> operator&(std::span<std::byte> a, std::span<std::byte> b) {
    std::vector<std::byte> output(a.size());

    for (int i = 0; i < a.size(); i++) {
        output[i] = a[i] & b[i];
    }

    return output;
}

int main() {
    std::byte a[3]{}, b[3]{};

    std::span<std::byte, 3> sa = a, sb = b;

    operator&(a, b);
    sa & b;

    return 0;
}
