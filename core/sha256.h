#pragma once

#include <cstdint>
#include <algorithm>

// SHA-256 Initial Hash Values (H0 to H7)
// constexpr std::uint32_t H[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
//                                            0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

// SHA-256 constants (first 32 bits of the cube roots of the first 64 primes)

const std::uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

#define ARRAY_TO_WORD(value, idxBegin) (value[idxBegin] << 24) | (value[idxBegin + 1] << 16) | (value[idxBegin + 2] << 8) | value[idxBegin + 3]

#define ROTR(x, n) ((x >> n) | (x << (-n & 31)))

#define SHR(x, n) (x >> n)

#define Ch(x, y, z) ((x & y) ^ (~x & z))

#define Maj(x, y, z) ((x & (y | z)) | (y & z))

#define s0(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10))

#define s1(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ SHR(x, 3))

#define Hcompute(a, b, c, d, e, f, g, h, T_1, T_2, K, W)                          \
    {                                                                             \
        T_1 = h + (ROTR(e, 6) ^ ROTR(e, 11) ^ ROTR(e, 25)) + Ch(e, f, g) + K + W; \
        T_2 = (ROTR(a, 2) ^ ROTR(a, 13) ^ ROTR(a, 22)) + Maj(a, b, c);            \
        h = g;                                                                    \
        g = f;                                                                    \
        f = e;                                                                    \
        e = d + T_1;                                                              \
        d = c;                                                                    \
        c = b;                                                                    \
        b = a;                                                                    \
        a = T_1 + T_2;                                                            \
    }

void sha256_round(std::uint32_t (&output)[8], std::uint8_t const *src)
{

    std::uint32_t W[64] = {0};

    for (std::size_t i = 0; i < 16; i++)
    {
        W[i] = ARRAY_TO_WORD(src, i * 4);
    }

    for (std::size_t i = 16; i < 64; i++)
    {
        W[i] = s0(W[i - 2]) + W[i - 7] + s1(W[i - 15]) + W[i - 16];
    }

    std::uint32_t a = output[0];
    std::uint32_t b = output[1];
    std::uint32_t c = output[2];
    std::uint32_t d = output[3];
    std::uint32_t e = output[4];
    std::uint32_t f = output[5];
    std::uint32_t g = output[6];
    std::uint32_t h = output[7];

    for (std::size_t i = 0; i < 64; i++)
    {
        std::uint32_t T_1 = 0;
        std::uint32_t T_2 = 0;
        Hcompute(a, b, c, d, e, f, g, h, T_1, T_2, K[i], W[i]);
    }

    output[0] += a;
    output[1] += b;
    output[2] += c;
    output[3] += d;
    output[4] += e;
    output[5] += f;
    output[6] += g;
    output[7] += h;
}

void sha256(std::uint32_t (&output)[8], std::uint8_t const *src, std::uint32_t n)
{
    output[0] = 0x6a09e667;
    output[1] = 0xbb67ae85;
    output[2] = 0x3c6ef372;
    output[3] = 0xa54ff53a;
    output[4] = 0x510e527f;
    output[5] = 0x9b05688c;
    output[6] = 0x1f83d9ab;
    output[7] = 0x5be0cd19;

    std::uint64_t bits_len = static_cast<std::uint64_t>(n * 8);
    std::uint32_t offset = 0;

    while (n >= 64)
    {
        sha256_round(output, src + offset);
        n -= 64;
        offset += 64;
    }

    // For less than 64 bytes block
    std::uint8_t last_block[64] = {0};
    std::copy_n(src + offset, n, last_block);
    last_block[n] = 0x80;

    if (n >= 56)
    {
        sha256_round(output, last_block);
        std::fill_n(last_block, 64, 0);
    }

    last_block[56] = bits_len >> 56;
    last_block[57] = bits_len >> 48;
    last_block[58] = bits_len >> 40;
    last_block[59] = bits_len >> 32;
    last_block[60] = bits_len >> 24;
    last_block[61] = bits_len >> 16;
    last_block[62] = bits_len >> 8;
    last_block[63] = bits_len;

    sha256_round(output, last_block);
}
