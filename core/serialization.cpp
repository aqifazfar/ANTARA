#include <vector>
#include <cstdint>
#include <algorithm>

#include "crc32.h"

#include "ANTARA/Core"

inline std::vector<std::uint8_t> Antara::Serialization(const std::uint8_t controlFlags, const std::uint8_t (&ID)[4], const std::uint8_t messageID, std::vector<uint8_t> &payload)
{
    std::vector<std::uint8_t> serialized = {controlFlags, ID[0], ID[1], ID[2], ID[3], this->sequenceNumber, messageID};

    serialized.push_back(payload.size());

    std::copy(payload.begin(), payload.end(), serialized.begin() + 8);

    std::uint8_t *arr = payload.data();

    std::uint8_t crc = crc32(arr, payload.size());

    serialized.push_back(crc);

    if (this->keySign != 0)
    {
        for (std::size_t i = 0; i < 4; i++)
        {
            serialized.push_back(this->keySign & (0xFF << 8 * (3 - i)));
        }
    }

    return serialized;
}