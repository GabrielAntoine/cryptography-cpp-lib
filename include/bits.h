#pragma once

#include <bitset>

template<size_t outputSize, size_t inputSize>
auto slice(const std::bitset<inputSize> &input, const size_t offset = 0) 
-> std::bitset<outputSize>;

template<size_t padding, size_t inputSize>
auto lpad(const std::bitset<inputSize> &input)
-> std::bitset<inputSize + padding>;

template<size_t padding, size_t inputSize>
auto rpad(const std::bitset<inputSize> &input)
-> std::bitset<inputSize + padding>;

template<size_t paddingLeft, size_t paddingRight, size_t inputSize>
auto pad(const std::bitset<inputSize> &input) 
-> std::bitset<inputSize + paddingLeft + paddingRight>;

template<size_t size>
auto rotl(const std::bitset<size> &bitset, const size_t shift)
-> std::bitset<size>;

template<size_t size>
auto rotr(const std::bitset<size> &bitset, const size_t shift)
-> std::bitset<size>;

#include "bits.tpp"