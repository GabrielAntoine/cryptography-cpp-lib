# Cryptography library in C++

![C++20](https://img.shields.io/badge/C++-20-blue)
![CMake](https://img.shields.io/badge/CMake-3.20+-brightgreen)
![Catch2](https://img.shields.io/badge/Testing-Catch2-orange)

This project is a C++ library containing my personal implementation of popular cryptography algorithms.

This project was created for fun and learning purposes.
Development stopped as I moved on to other things.

## Warning

⚠️ Do **NOT** use it in your production environment! This is a hobby project which is therefore unreliable for secure contexts. Instead, use a real library such as [OpenSSL](https://www.openssl.org/).

## Architecture

There is 0 runtime polymorphism, 0 runtime inheritance.

This project uses only templates and [C++20 concepts](https://en.cppreference.com/w/cpp/language/constraints.html) as its way of handling abstraction. This approach enforces strong compile-time guarantees and can improve performance.

The drawback is that most files are templates, aka hard syntax and slow compile time.

## Examples

Here are a few examples to show you how it works. For more examples, refer to the [tests](/test/encryption.cpp)

> **Note** : The type `ByteArray<N>` is an alias of `std::array<std::byte, N>` or `std::vector<std::byte>` if `N` is `std::dynamic_extent`

### DES/CBC/PKCS5Padding

```cpp
ByteArray<15> plainText = toByteArrayFromAscii("gabriel le goat");
ByteArray<8> iv = toByteArrayFromHexa("0123456789abcdef");
ByteArray<DES::SecretKey::KEY_SIZE_BYTES> rawKey = toByteArrayFromHexa("133457799bbcdff1");

DES des;
CBC<DES> cbc;
PKCS5Padding padding;

des.setKey(DES::SecretKey(rawKey))
cbc.setIV(iv);

// Generic abstraction that expects :
// 1. An encryption algorithm
// 2. A mode algorithm
// 3. A padding algorithm
BlockCipher cipher(des, cbc, padding);

// Output is : 0377B18F0ACCCD1A87F3491E478F4B4C
auto encrypted = cipher.encrypt(plainText);
std::cout << toString(encrypted) << '\n';

// Output is : true
auto decrypted = cipher.decrypt(encrypted);
std::cout << (toString(encrypted) == toString(plainText))<< '\n';
```

### AES/GCM

```cpp
ByteArray<16> rawKey = toByteArrayFromHexa("000102030405060708090a0b0c0d0e0f");
ByteArray<16> iv = toByteArrayFromHexa("0123456789abcdef1011121314151617");
ByteArray<51> aad = toByteArrayFromAscii("This is an example of additional authenticated data");
ByteArray<34> plainText = toByteArrayFromAscii("No mind to think, to will to break");

AES<128> aes;
GCM<128> gcm;

// It won't compile if the AES<N> object and the AESSecretKey<N>
// object don't have the same value for N. It won't also compile
// if N is anything other than 128, 192 and 256, in accordance
// with the AES specification. Same for the GCM<N> object.
aes.setKey(AESSecretKey<128>(rawKey));
gcm.setIV(iv);
gcm.setAAD(aad);

BlockCipher cipher(aes128, gcm, NoPadding());

// Output is : DFA25B13D64C7701D4E6B01A3CE76AA1E36FB1BB660B1C0BADA5F8D697388D3B31E651A2603866D28CB910581BFC74B177D1
auto encrypted = cipher.encrypt(plainText);
std::cout << toString(encrypted) << '\n';

// Output is : true
auto decrypted = cipher.decrypt(encrypted);
std::cout << (toString(encrypted) == toString(plainText))<< '\n';
```

## Project structure

This project uses [CMake](https://cmake.org/) for build management and [Catch2](https://catch2.org/) for testing.

Coming with a good project structure in C++ in honestly one of the hardest things I've ever faced, and I'm not satisfied _at all_.

There's plenty reasons for that. I spend a long time wondering how to handle the `.h` and `.cpp` files. Having everything kind of duplicated make the file structure much harder to read.

C++ dependency management can be challenging. I managed to install Catch2, but it's still very confusing to me how to properly add dependancies and how to transform this project into its own library.

I decided to organize my files this way :

### 📂 src/

The `.cpp` files organized in subfolders by subject.

### 📂 include/

Every `.h` file in the same directory. Why ? Two reasons :

1. The project is already organized in the `src/` directory.
2. The C++ linter must know where to find every included file, which means I didn't have to put a separate entry for each folder inside `include/` in `CMakeLists.txt`.

### 📂 template/

The same as `src/` but for `.tpp` files. These are sources files that have to be included, which is actually most of my code.

The way I separated the code files in three folders is the best I could come with, but I'm not satisfied because it's still very messy.

### 📂 test/

The tests using Catch2

## Algorithms implemented

- Encryption algorithms : DES, 3DES, AES
- Mode algorithms : ECB, CBC, CFB, OFB, CTR, GCM
- Paddings algorithms : PKCS5Padding, PKCS7Padding, ZeroPadding, NoPadding

I planned to implement MD5, SHA-256 and RSA, but I'll probably never do it.

## Quick start

### Requirements

- C++20 compatible compiler
- CMake ≥ 3.20
- Ninja (optional but recommended)

### Install

1. Make sure to have a C++ compiler.
   ```sh
   sudo apt install build-essential
   ```
2. Install CMake [here](https://cmake.org/download/).
3. Install Ninja [here](https://ninja-build.org/).
4. At the root of the project, configure CMake :
   ```sh
   cmake -B build -G Ninja .
   ```
5. Build the project :
   ```sh
   cmake --build build
   ```
6. Run the tests :
   ```sh
   ctest --test-dir build
   ```

## Usage

There's nothing you can really try except running the tests as I didn't take the time to package this project as a proper library.

```sh
ctest --test-dir build
```
