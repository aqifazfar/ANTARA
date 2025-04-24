#pragma once

#include <cstdint>
#include <algorithm>
#include <iostream>
#include <atomic>

#include "antara_types.h"
#include "crc32.h"
#include "sha256.h"

#define WORD_TO_ARRAY(output, value, idxBegin)   \
    output[idxBegin + 0] = (value >> 24) & 0xFF; \
    output[idxBegin + 1] = (value >> 16) & 0xFF; \
    output[idxBegin + 2] = (value >> 8) & 0xFF;  \
    output[idxBegin + 3] = (value & 0xFF);

class Serialization
{

private:
    Serialization() = default;
    ~Serialization() = default;

private:
    inline static std::atomic<std::uint32_t> sequence_number = 1;

public:
    static Serialization &Get_Instance()
    {
        static Serialization instance;
        return instance;
    }

    inline static std::uint8_t Get_Current_Sequence()
    {
        return sequence_number;
    }

    template <std::size_t messageLength>
    inline static antara_msg_t<messageLength> Serialize(std::uint8_t controlFlags, std::uint32_t groupID, std::uint32_t systemID, antara_payload_t const &payload)
    {

        antara_msg_t<messageLength> output;

        output[0] = controlFlags;

        WORD_TO_ARRAY(output, groupID, 1);
        WORD_TO_ARRAY(output, systemID, 5);

        output[9] = sequence_number.fetch_add(1);
        output[10] = payload.nodeID;
        output[11] = payload.componentID;
        output[12] = (payload.messageID >> 8) & 0xFF;
        output[13] = payload.messageID & 0xFF;
        output[14] = payload.length;

        for (std::size_t i = 0; i < payload.length; i++)
        {
            output[15 + i] = payload.data[i];
        }

        std::uint32_t crc = crc32(output.msg, 15 + payload.length);

        WORD_TO_ARRAY(output, crc, 15 + payload.length);

        if ((controlFlags & 0x03) == SIGNATURE_EXTENSION)
        {
            std::uint32_t hash[8];
            std::uint16_t index = 19 + payload.length;

            sha256<messageLength - 32>(hash, output.msg);

            for (std::size_t i = 0; i < 8; i++)
            {
                WORD_TO_ARRAY(output, hash[i], index);
                index += 4;
            }
        }

        return output;
    }

    Serialization(Serialization const &) = delete;
    Serialization &operator=(Serialization const &) = delete;
    Serialization(Serialization &&) = delete;
    Serialization &operator=(Serialization &&) = delete;
};
