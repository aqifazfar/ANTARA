#pragma once

#include <cstdint>
#include <algorithm>
#include <atomic>

#include "ANTARA/antara_types.h"
#include "crc32.h"
#include "sha256.h"

#define ARRAY_TO_WORD(value, idxBegin) (value[idxBegin] << 24) | (value[idxBegin + 1] << 16) | (value[idxBegin + 2] << 8) | value[idxBegin + 3]

class Deserialization
{

private:
    std::atomic<std::uint32_t> sequence_number = 1;

public:
    Deserialization() {};
    ~Deserialization() {};

    template <std::size_t messageLength>
    inline int Deserialize(std::uint32_t const &groupID, std::uint32_t const &systemID, antara_mode const &mode, std::uint16_t const &subsMessageID, antara_payload_t &output, antara_msg_t<messageLength> const &msgIn)
    {
        if constexpr (messageLength > 27)
        {
            if (crc32(msgIn.msg, messageLength - 8) != 0)
            {
                // Discard message
                return MESSAGE_CORRUPTED;
            }

            // Check Signature
            if ((msgIn[0] & 0x03) == SIGNATURE_EXTENSION)
            {
                std::uint32_t hash[8];
                sha256(hash, msgIn.msg, messageLength - 32);

                std::uint32_t index = 19 + msgIn[14];

                for (std::size_t i = 0; i < 2; i++)
                {
                    std::uint32_t temp;
                    temp = ARRAY_TO_WORD(msgIn, index);

                    if (temp != hash[i])
                    {
                        // Discard message
                        return 0;
                    }

                    index += 4;
                }
            }
        }
        else
        {
            if (crc32(msgIn.msg, messageLength) != 0)
            {
                // Discard message
                return 0;
            }
        }

        std::uint32_t msg_group_ID = (msgIn[1] << 24) | (msgIn[2] << 16) | (msgIn[3] << 8) | msgIn[4];
        std::uint32_t msg_system_ID = (msgIn[5] << 24) | (msgIn[6] << 16) | (msgIn[7] << 8) | msgIn[8];
        std::uint16_t msg_msg_ID = (msgIn[12] << 8) | msgIn[13];
        std::uint8_t msg_mode = msgIn[0] & 0xC0;

        // Check mode
        if ((msg_mode == UNICAST) && (mode == UNICAST))
        {
            if ((msg_group_ID != groupID) || (msg_system_ID != systemID))
            {
                // Discard Message
                return 0;
            }
        }

        else if ((msg_mode == MULTICAST) && (mode == MULTICAST))
        {
            if ((msg_group_ID != groupID) && (msg_msg_ID != subsMessageID))
            {
                return 0;
            }
        }

        else if ((msg_mode == BROADCAST) && (mode == BROADCAST))

        {
            // Broadcast message
            if (msg_msg_ID != subsMessageID)
            {
                return 0;
            }
        }

        else
        {
            // wrong argument but will still proceed with unicast message
            return 0;
        }

        if (msgIn[9] != sequence_number)
        {
            return MESSAGE_MISSING;
        }

        output.nodeID = msgIn[10];
        output.componentID = msgIn[11];
        output.messageID = msg_msg_ID;
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