#include <array>
#include <cstddef>
#include <functional>
#include <cstdint>
#include <span>
#include <vector>
#include <algorithm>

#include "bytes.h"


// ===================================================================================
// SECTION : Operators &  |  ^  ~
// ===================================================================================

// ByteArray &doBytesToBytesOperation(
//     ByteArray &left, 
//     const ByteArray &right, 
//     ByteToByteOperation doByteToByteOperation
// ) {
//     const size_t outputSize = std::min(left.size(), right.size());

//     for (int i = 0; i < outputSize; i++) {
//         left[i] = doByteToByteOperation(left[i], right[i]);
//     }

//     return left;
// }

// ByteArray& operator&=(ByteArray &left, const ByteArray &right) {
//     return doBytesToBytesOperation(left, right, std::bit_and());
// }

// ByteArray operator&(const ByteArray& left, const ByteArray &right) {
//     ByteArray output = left;
//     return output &= right;
// }

// ByteArray operator&(const ByteArray& left, const size_t right) {
//     return left & toByteArray(right);
// }

// ByteArray& operator|=(ByteArray &left, const ByteArray &right) {
//     return doBytesToBytesOperation(left, right, std::bit_or());
// }

// ByteArray operator|(const ByteArray& left, const ByteArray &right) {
//     ByteArray output = left;
//     return output |= right;
// }

// ByteArray operator|(const ByteArray& left, const size_t right) {
//     return left | toByteArray(right);
// }

// ByteArray& operator^=(ByteArray &left, const ByteArray &right) {
//     return doBytesToBytesOperation(left, right, std::bit_xor());
// }

// ByteArray operator^(const ByteArray& left, const ByteArray &right) {
//     ByteArray output = left;
//     return output ^= right;
// }

// ByteArray operator^(const ByteArray& left, const size_t right) {
//     return left ^ toByteArray(right);
// }

// ByteArray operator~(const ByteArray &bytes) {
//     ByteArray output(bytes.size());

//     std::transform(bytes.begin(), bytes.end(), output.begin(), std::bit_not());

//     return output;
// }



// // ===================================================================================
// // SECTION : Operators <<  >>
// // ===================================================================================

// ByteArray operator<<(const ByteArray &bytes, const size_t shift) {
//     if (shift < 0) {
//         return bytes >> (-shift);
//     }

//     const std::uint8_t bitsInTheLeftBlock  = shift % BITS_PER_BYTE;
//     const std::uint8_t bitsInTheRightBlock = BITS_PER_BYTE - bitsInTheLeftBlock;
//     const size_t       blockShift          = shift / BITS_PER_BYTE;
//     ByteArray output(bytes.size());

//     for (int i = 0; i < bytes.size(); i++) {
//         std::byte block        = bytes[i];
//         std::byte leftBlock    = block >> bitsInTheRightBlock;
//         std::byte rightBlock   = block << bitsInTheLeftBlock;
//         size_t rightBlockIndex = i - blockShift;
//         size_t leftBlockIndex  = rightBlockIndex - 1;

//         if (leftBlockIndex >= 0) {
//             output[leftBlockIndex] |= leftBlock;
//         }
//         if (rightBlockIndex >= 0) {
//             output[rightBlockIndex] |= rightBlock;
//         }
//     }

//     return output;
// }

// ByteArray operator>>(const ByteArray &bytes, const size_t shift) {
//     if (shift < 0) {
//         return bytes << (-shift);
//     }

//     const std::uint8_t bitsInTheRightBlock = shift % BITS_PER_BYTE;
//     const std::uint8_t bitsInTheLeftBlock  = BITS_PER_BYTE - bitsInTheRightBlock;
//     const size_t       blockShift          = shift / BITS_PER_BYTE; 
//     ByteArray output(bytes.size());

//     for (int i = bytes.size() - 1; i >= 0; i--) {
//         std::byte block        = bytes[i];
//         std::byte leftBlock    = block >> bitsInTheRightBlock;
//         std::byte rightBlock   = block << bitsInTheLeftBlock;
//         size_t leftBlockIndex  = i + blockShift;
//         size_t rightBlockIndex = leftBlockIndex + 1;

//         if (leftBlockIndex < bytes.size()) {
//             output[leftBlockIndex] |= leftBlock;
//         }
//         if (rightBlockIndex < bytes.size()) {
//             output[rightBlockIndex] |= rightBlock;
//         }
//     }

//     return output;
// }

// ByteArray& operator<<=(ByteArray &bytes, const size_t shift) {
//     return bytes = bytes << shift;
// }

// ByteArray& operator>>=(ByteArray &bytes, const size_t shift) {
//     return bytes = bytes >> shift;
// }

// ByteArray rotl(const ByteArray &bytes, const size_t shift) {
//     const size_t bitsCount = bytes.size() * BITS_PER_BYTE;
//     const size_t localShift = shift % bitsCount;

//     if (localShift == 0) {
//         return bytes;
//     } else if (localShift > 0) {
//         return (bytes << localShift) | (bytes >> (bitsCount - localShift));
//     } else {
//         return rotr(bytes, -localShift);
//     }
// }

// ByteArray rotr(const ByteArray &bytes, const size_t shift) {
//     const size_t bitsCount = bytes.size() * BITS_PER_BYTE;
//     const size_t localShift = shift % bitsCount;

//     if (localShift == 0) {
//         return bytes;
//     } else if (localShift > 0) {
//         return (bytes >> localShift) | (bytes << (bitsCount - localShift));
//     } else {
//         return rotr(bytes, -localShift);
//     }
// }





// // ===================================================================================
// // SECTION : Utils
// // ===================================================================================

// bool bitAt(const ByteArray &bytes, std::size_t index) {
//     return static_cast<bool>((bytes[index / BITS_PER_BYTE] >> (index % BITS_PER_BYTE)) & std::byte(1));
// }

ByteArray toByteArray(const size_t n) {
    ByteArray output(sizeof n);

    for (int i = sizeof n - 1; i >= 0; i--) {
        output[sizeof n - i - 1] = std::byte((n >> (CHAR_BIT * i)) & UINT8_MAX);
    }

    return output;
}