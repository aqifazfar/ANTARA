#include <iostream>
#include "Antara/Core"

class heartbeat
{

private:
    inline static std::uint8_t currentSequence = 0;

public:
    inline static antara_msg_t<59> Pack(std::uint32_t const &groupID, std::uint32_t const &systemID, std::uint8_t const (&data)[8])
    {
        antara_msg_t<59> output = {};
        antara_payload_t payload = {10, 200, 1, 8, {data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]}};

        output = Serialization::Serialize<59>(RELIABLE_MODE | PUB_SUB | SIGNATURE_EXTENSION, groupID, systemID, payload);

        // To track the sequence for reliable mode
        currentSequence = Serialization::Get_Instance().Get_Current_Sequence();

        return output;
    }

    inline static int Unpack(std::uint32_t const &groupID, std::uint32_t const &systemID, antara_payload_t &output, antara_msg_t<59> const &msgIn)
    {
        int status = Deserialization::Deserialize<59>(groupID, systemID, output, msgIn);

        return status;
    }
};

int main()
{
    antara_msg_t<59> hbeat = heartbeat::Pack(20, 250, {1, 2, 3, 4, 5, 6, 7, 8});

    std::cout << hbeat;

    hbeat = heartbeat::Pack(20, 250, {1, 2, 3, 4, 5, 6, 7, 8});

    antara_payload_t out;

    int stat = heartbeat::Unpack(20, 250, out, hbeat);

    std::cout << stat << "\n";

    std::cout << out;

    return 0;
}