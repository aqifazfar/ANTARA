#pragma once

#include <cstdint>
#include <algorithm>
#include <atomic>

#include "antara_types.h"
#include "crc32.h"
#include "sha256.h"

#define ARRAY_TO_WORD(output, value, idxBegin) \
    output = (value[idxBegin] << 24) | (value[idxBegin + 1] << 16) | (value[idxBegin + 2] << 8) | value[idxBegin + 3];

class Deserialization
{

private:
    Deserialization() = default;
    ~Deserialization() = default;

    inline static std::atomic<std::uint32_t> sequence_number = 1;

public:
    static Deserialization &Get_Instance()
    {
        static Deserialization instance;
        return instance;
    }

    template <std::size_t messageLength>
    inline static int Deserialize(std::uint32_t const &groupID, std::uint32_t const &systemID, antara_payload_t &output, antara_msg_t<messageLength> const &msgIn)
    {

        std::uint32_t msg_group_ID = (msgIn[1] << 24) | (msgIn[2] << 16) | (msgIn[3] << 8) | msgIn[4];
        std::uint32_t msg_system_ID = (msgIn[5] << 24) | (msgIn[6] << 16) | (msgIn[7] << 8) | msgIn[8];

        if ((msg_group_ID != groupID) || (msg_system_ID != systemID))
        {
            // Discard Message
            return 0;
        }

        // Check Signature
        if ((msgIn[0] & 0x03) == SIGNATURE_EXTENSION)
        {
            std::uint32_t hash[8];
            sha256<messageLength - 32>(hash, msgIn.msg);

            std::uint32_t index = 19 + msgIn[14];

            for (std::size_t i = 0; i < 8; i++)
            {
                std::uint32_t temp;
                ARRAY_TO_WORD(temp, msgIn, index);

                if (temp != hash[i])
                {
                    // discard message
                    return 0;
                }

                index += 4;
            }
        }

        if (crc32(msgIn.msg, messageLength - 32) != 0)
        {

            return MESSAGE_CORRUPTED;
        }

        if (msgIn[9] != sequence_number)
        {
            return MESSAGE_MISSING;
        }

        output.nodeID = msgIn[10];
        output.componentID = msgIn[11];
        output.messageID = (msgIn[12] << 8) | msgIn[13];
        output.length = msgIn[14];

        for (std::size_t i = 0; i < output.length; i++)
        {
            output.data[i] = msgIn[15 + i];
        }

        sequence_number.fetch_add(1);

        return 0;
    }

    Deserialization(Deserialization const &) = delete;
    Deserialization &operator=(Deserialization const &) = delete;
    Deserialization(Deserialization &&) = delete;
    Deserialization &operator=(Deserialization &&) = delete;
};