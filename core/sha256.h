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

template <std::uint8_t n>
inline static std::uint32_t ROTR(std::uint32_t const &x)
{
    static_assert(n < 32, "Rotate more than 31 bits. ABORT.\n");

    return (x >> n) | (x << (-n & 31));
}

template <std::uint8_t n>
inline static std::uint32_t SHR(std::uint32_t const &x)
{
    static_assert(n < 32, "Shift more than 31 bits. ABORT.\n");

    return (x >> n);
}

inline static std::uint32_t Ch(std::uint32_t const &x, std::uint32_t const &y, std::uint32_t const &z)
{
    return ((x & y) ^ (~x & z));
}

inline static std::uint32_t Maj(std::uint32_t const &x, std::uint32_t const &y, std::uint32_t const &z)
{
    return ((x & (y | z)) | (y & z));
}

template <std::uint32_t length2Digest>
inline void sha256(std::uint32_t *out, std::uint8_t const *src)
{

    constexpr std::uint32_t padding_msg_size = 64 * ((length2Digest + 72) / 64);

    std::uint8_t msg_padded[padding_msg_size] = {0};

    std::copy_n(src, length2Digest, msg_padded);

    msg_padded[length2Digest] = 0x80;

    std::uint64_t bits_length2Digest = static_cast<std::uint64_t>(length2Digest * 8);
    for (std::size_t i = 0; i < 8; i++)
    {
        msg_padded[padding_msg_size - 8 + i] = bits_length2Digest >> (8 * (7 - i));
    }

    std::uint32_t H[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                          0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

    std::uint32_t M_n = padding_msg_size / 64;

    for (std::uint32_t M = 0; M < M_n; M++)
    {
        std::uint32_t a, b, c, d, e, f, g, h;

        std::uint32_t W[64] = {0};

        // W[0:15] = M[0:15]
        for (std::size_t i = 0; i < 16; i++)
        {
            W[i] = (msg_padded[M * 64 + i * 4] << 24) | (msg_padded[M * 64 + i * 4 + 1] << 16) | (msg_padded[M * 64 + i * 4 + 2] << 8) | msg_padded[M * 64 + i * 4 + 3];
        }

        //  W[16:63] = sigma_1(W[i-2]) + W[i-7] + sigma_0(W[i-15]) +W[i-16]
        for (std::size_t i = 16; i < 64; i++)
        {
            W[i] = (ROTR<17>(W[i - 2]) ^ ROTR<19>(W[i - 2]) ^ SHR<10>(W[i - 2])) + W[i - 7] + (ROTR<7>(W[i - 15]) ^ ROTR<18>(W[i - 15]) ^ SHR<3>(W[i - 15])) + W[i - 16];
        }

        a = H[0];
        b = H[1];
        c = H[2];
        d = H[3];
        e = H[4];
        f = H[5];
        g = H[6];
        h = H[7];

        for (std::size_t i = 0; i < 64; i++)
        {

            std::uint32_t T_1 = h + (ROTR<6>(e) ^ ROTR<11>(e) ^ ROTR<25>(e)) + Ch(e, f, g) + K[i] + W[i];
            std::uint32_t T_2 = (ROTR<2>(a) ^ ROTR<13>(a) ^ ROTR<22>(a)) + Maj(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T_1;
            d = c;
            c = b;
            b = a;
            a = T_1 + T_2;
        }

        H[0] += a;
        H[1] += b;
        H[2] += c;
        H[3] += d;
        H[4] += e;
        H[5] += f;
        H[6] += g;
        H[7] += h;
    }

    out[0] = H[0];
    out[1] = H[1];
    out[2] = H[2];
    out[3] = H[3];
    out[4] = H[4];
    out[5] = H[5];
    out[6] = H[6];
    out[7] = H[7];
}