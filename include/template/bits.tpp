#include "bits.h"

template<size_t outputSize, size_t inputSize>
auto slice(const std::bitset<inputSize> &input, const size_t offset) -> std::bitset<outputSize> {
    assert(offset + outputSize <= inputSize); // outputSize can be checked at compile time but offset cannot

    std::bitset<outputSize> output;

    for (size_t i = 0; i < outputSize; i++) {
        output[i] = input[inputSize - offset - outputSize + i];
    }

    return output;
}

template<size_t padding, size_t inputSize>
auto lpad(const std::bitset<inputSize> &input) -> std::bitset<inputSize + padding> {
    std::bitset<inputSize + padding> output;

    for (size_t i = 0; i < inputSize; i++) {
        output[i] = input[i];
    }

    return output;
}

template<size_t padding, size_t inputSize>
auto rpad(const std::bitset<inputSize> &input) -> std::bitset<inputSize + padding> {
    std::bitset<inputSize + padding> output;

    for (size_t i = 0; i < inputSize; i++) {
        output[i + padding] = input[i];
    }

    return output;
}

template<size_t paddingLeft, size_t paddingRight, size_t inputSize>
auto pad(const std::bitset<inputSize> &input) -> std::bitset<inputSize + paddingLeft + paddingRight> {
    return rpad<paddingRight>(lpad<paddingLeft>(input));
}

template<size_t size>
auto rotl(const std::bitset<size> &bitset, const size_t shift) -> std::bitset<size> {
    const size_t localShift = shift % size;

    if (localShift == 0) {
        return bitset;
    }

    return (bitset << localShift) | (bitset >> (size - localShift));
}

template<size_t size>
auto rotr(const std::bitset<size> &bitset, const size_t shift) -> std::bitset<size> {    
    const size_t localShift = shift % size;

    if (localShift == 0) {
        return bitset;
    }

    return (bitset << localShift) | (bitset << (size - localShift));
}

template <size_t size>
auto increment(std::bitset<size> &bits) -> std::bitset<size>& {
    bool hasCarry = true;

    for (size_t i = 0; i < size && hasCarry; ++i) {
        hasCarry = bits[i];
        bits.flip(i);
    }

    return bits;
}