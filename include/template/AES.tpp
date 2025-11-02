#include "AES.h"

template <size_t keySize>
    requires AESKeySize<keySize>
void AES<keySize>::setKey(const SecretKey &key) {
    this->key = key;
}

template <size_t keySize>
    requires AESKeySize<keySize>
std::byte& AES<keySize>::matrixAt(Matrix& matrix, const int col, const int row) const {
    return matrix[col * MATRIX_LENGTH + row];
}

template <size_t keySize>
    requires AESKeySize<keySize>
void AES<keySize>::addRoundKey(Matrix &matrix, const int round) const {
    typename SecretKey::RoundKey roundKey = key.getRoundKey(round);

    using B = ByteSpan<BLOCK_SIZE_BYTES>;

    matrix = B(matrix) ^ B(roundKey);
}


template <size_t keySize>
    requires AESKeySize<keySize>
template <AES_TABLES::SBOX_TYPE sbox>
void AES<keySize>::substituteBytes(Matrix &matrix) const {
    for (int c = 0; c < MATRIX_LENGTH; c++) {
        for (int l = 0; l < MATRIX_LENGTH; l++) {
            uint8_t byte = std::to_integer<uint8_t>(matrixAt(matrix, c, l));
            uint8_t col  = byte & 0x0F;
            uint8_t row  = (byte & 0xF0) >> (CHAR_BIT / 2);

            matrixAt(matrix, c, l) = std::byte(sbox[row][col]);
        }
    }
}

template <size_t keySize>
    requires AESKeySize<keySize>
void AES<keySize>::shiftRows(Matrix &matrix) const {
    // Row 1
    for (int c = MATRIX_LENGTH - 1; c >= 0; c--) {
        std::swap(matrixAt(matrix, c, 1), matrixAt(matrix, 0, 1));
    }

    // Row 2
    std::swap(matrixAt(matrix, 0, 2), matrixAt(matrix, 2, 2));
    std::swap(matrixAt(matrix, 1, 2), matrixAt(matrix, 3, 2));

    // Row 3
    for (int c = 0; c < MATRIX_LENGTH; c++) {
        std::swap(matrixAt(matrix, c, 3), matrixAt(matrix, 3, 3));
    }
}

template <size_t keySize>
    requires AESKeySize<keySize>
void AES<keySize>::invShiftRows(Matrix &matrix) const {
    // Row 1
    for (int c = 0; c < MATRIX_LENGTH; c++) {
        std::swap(matrixAt(matrix, c, 1), matrixAt(matrix, 0, 1));
    }

    // Row 2
    std::swap(matrixAt(matrix, 0, 2), matrixAt(matrix, 2, 2));
    std::swap(matrixAt(matrix, 1, 2), matrixAt(matrix, 3, 2));

    // Row 3
    for (int c = MATRIX_LENGTH - 1; c >= 0; c--) {
        std::swap(matrixAt(matrix, c, 3), matrixAt(matrix, 3, 3));
    }
}

template <size_t keySize>
    requires AESKeySize<keySize>
template <AES_TABLES::MIX_COLUMNS_MATRIX_TYPE mixColumnsMatrix>
void AES<keySize>::mixColumns(Matrix &matrix) const {
    for (int word = 0; word < MATRIX_LENGTH; word++) {
        ByteArray<MATRIX_LENGTH> processingColumn{std::byte(0)};

        // Galois matrix product betwen constant matrix and column of index 'word'
        for (int matrixRow = 0; matrixRow < MATRIX_LENGTH; matrixRow++) {
            for (int k = 0; k < MATRIX_LENGTH; k++) {
                std::byte mixColumnsMatrixByte = mixColumnsMatrix[matrixRow][k];
                std::byte wordByte = matrixAt(matrix, word, k);

                processingColumn[matrixRow] ^= Galois256::multiply(mixColumnsMatrixByte, wordByte);
            }
        }
        
        // Copy the result of the matrix product in the state matrix
        for (int i = 0; i < MATRIX_LENGTH; i++) {
            matrixAt(matrix, word, i) = processingColumn[i];
        }
    }
}

template <size_t keySize>
    requires AESKeySize<keySize>
AES<keySize>::Block AES<keySize>::encrypt(const Block &plaintext) const {
    Matrix matrix = toByteArray(plaintext);

    // Initial round
    addRoundKey(matrix, 0);

    // 10, 12 or 14 rounds (depending on the key size)
    // i = 1 and ROUND_COUNT - 1 because the initial and last rounds are included in the ROUND_COUNT constant
    for (int i = 1; i < SecretKey::ROUND_COUNT - 1; i++) {
        substituteBytes<AES_TABLES::SBOX>(matrix);
        shiftRows(matrix);
        mixColumns<AES_TABLES::MIX_COLUMNS_MATRIX>(matrix);
        addRoundKey(matrix, i);
    }

    // Last round
    substituteBytes<AES_TABLES::SBOX>(matrix);
    shiftRows(matrix);
    addRoundKey(matrix, SecretKey::ROUND_COUNT - 1); // 0-indexed

    Block ciphertext = toBitset(ByteSpan<BLOCK_SIZE_BYTES>(matrix));

    return ciphertext;
}

template <size_t keySize>
    requires AESKeySize<keySize>
AES<keySize>::Block AES<keySize>::decrypt(const Block &plaintext) const {
    Matrix matrix = toByteArray(plaintext);

    // Initial round
    addRoundKey(matrix, SecretKey::ROUND_COUNT - 1);

    for (int i = SecretKey::ROUND_COUNT - 2; i > 0; i--) {
        invShiftRows(matrix);
        substituteBytes<AES_TABLES::INV_SBOX>(matrix);
        addRoundKey(matrix, i);
        mixColumns<AES_TABLES::INV_MIX_COLUMNS_MATRIX>(matrix);
        // std::cout << "istart : " << matrix << std::endl;
    }

    // Last round
    invShiftRows(matrix);
    substituteBytes<AES_TABLES::INV_SBOX>(matrix);
    addRoundKey(matrix, 0);
    
    Block ciphertext = toBitset(ByteSpan<BLOCK_SIZE_BYTES>(matrix));

    return ciphertext;
} 