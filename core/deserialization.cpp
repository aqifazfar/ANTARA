#include <cstdint>
#include <vector>

#include "crc32.h"

#include "ANTARA/Core"

inline std::vector<std::uint8_t> Antara::Deserialization(std::vector<std::uint8_t> &msgIn)
{

    // Check operation mode
    std::uint8_t mode = msgIn[0] & 0xC;
    if ((mode == REAL_TIME) || (mode == RELIABLE))
    {
        for (std::int8_t i = 0; i < 4; i++)
        {
            if (msgIn[1 + i] != this->ID[i])
            {
                return;
            }
        }
    }

    if (mode == MULTICAST)
    {
        if (msgIn[1] != this->ID[0])
        {
            return;
        }
    }

    std::uint8_t *temp = msgIn.data();

    std::uint32_t crc = crc32(temp, msgIn.size());

    for (std::size_t i = 0; i < 4; i++)
    {
        if (msgIn[msgIn.size() - 4 + i] != (crc & ((0xFF) << 8 * (3 - i))))
        {
            return;
        }
    }

    std::vector<std::uint8_t> payloadOut = {msgIn[6], msgIn[7]};

    for (std::size_t i = 0; i < msgIn[7]; i++)
    {
        payloadOut.push_back(msgIn[7 + i]);
    }

    return payloadOut;
}