#pragma once

#include <cstdint>
#include <iostream>

#include "antara_version.h"

enum antara_extension_flags
{
    SIGNATURE_EXTENSION = 0x01,

};

enum antara_mode
{
    UNICAST = 0x00,
    MULTICAST = 0x80,
    BROADCAST = 0x00,
};

enum antara_error_flags
{
    MESSAGE_CORRUPTED = -1,
    MESSAGE_MISSING = -2,

};

template <std::size_t messageLength>
struct antara_msg_t
{
    std::uint8_t msg[messageLength];

    std::uint8_t &operator[](std::size_t i)
    {
        return msg[i];
    }

    std::uint8_t const &operator[](std::size_t i) const
    {
        return msg[i];
    }

    friend std::ostream &operator<<(std::ostream &output, antara_msg_t const &in)
    {
        std::uint32_t groupID = (in[1] << 24) | (in[2] << 16) | (in[3] << 8) | in[4];
        std::uint32_t systemID = (in[5] << 24) | (in[6] << 16) | (in[7] << 8) | in[8];
        std::uint16_t messageID = (in[12] << 8) | in[13];
        std::uint32_t crc = (in[15 + in[14]] << 24) | (in[15 + in[14] + 1] << 16) | (in[15 + in[14] + 2] << 8) | in[15 + in[14] + 3];

        output << "Protocol Version: " << version[1 - (in[0] >> 4)] << "\n";
        output << "Control Flags: " << static_cast<std::uint32_t>(in[0] & 0xFF) << "\n";
        output << "Group ID: " << groupID << "\n";
        output << "System ID: " << systemID << "\n";
        output << "Sequence Number: " << static_cast<std::uint32_t>(in[9]) << "\n";
        output << "Node ID: " << static_cast<std::uint32_t>(in[10]) << "\n";
        output << "Component ID: " << static_cast<std::uint32_t>(in[11]) << "\n";
        output << "Message ID: " << messageID << "\n";
        output << "Message Length: " << static_cast<std::uint32_t>(in[14]) << "\n";
        output << "Data: ";

        for (std::size_t i = 0; i < in[14]; i++)
        {
            output << static_cast<std::uint32_t>(in[15 + i]) << " ";
        }
        output << "\n";
        output << std::hex << "CRC: " << crc << "\n";
        output << "Signature: ";

        for (std::size_t i = (15 + 4 + in[14]); i < messageLength; i++)
        {
            output << static_cast<std::uint32_t>(in[i]);
        }

        output << "\n------------------------------------------------------------------------------" << std::dec << std::endl;

        return output;
    }
};

struct antara_payload_t
{
    std::uint8_t nodeID = 0;
    std::uint8_t componentID = 0;
    std::uint16_t messageID = 0;
    std::uint8_t length = 0;
    uint8_t data[255] = {};

    friend std::ostream &operator<<(std::ostream &output, antara_payload_t const &in)
    {
        output << "Node ID: " << static_cast<std::uint32_t>(in.nodeID) << "\n";
        output << "Component ID: " << static_cast<std::uint32_t>(in.componentID) << "\n";
        output << "Message ID: " << static_cast<std::uint32_t>(in.messageID) << "\n";
        output << "Message Length: " << static_cast<std::uint32_t>(in.length) << "\n";
        output << "Data: ";

        for (std::size_t i = 0; i < in.length; i++)
        {
            output << static_cast<std::uint32_t>(in.data[i]) << " ";
        }

        output << "\n------------------------------------------------------------------------------" << std::endl;

        return output;
    }
};
