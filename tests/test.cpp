#include <iostream>
#include "Antara/Core"

inline antara_msg_t<59> heartbeat_pack(std::uint8_t const (&data)[8])
{
    antara_msg_t<59> output;
    antara_payload_t payload = {10, 200, 1, 8, {data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]}};

    output = Antara::Core::Get_Instance().Pack<59>(UNICAST | SIGNATURE_EXTENSION, payload);

    return output;
}

inline antara_payload_t heartbeat_unpack(antara_msg_t<59> const &msgIn)
{
    antara_payload_t output = Antara::Core::Get_Instance().Unpack<59>(msgIn);

    return output;
}

int main()
{
    Antara::Antara_Init();

    antara_msg_t<59> a = heartbeat_pack({1, 2, 3, 4, 5, 6, 7, 8});

    std::cout << a;

    antara_payload_t b = heartbeat_unpack(a);

    std::cout << b;

    std::cout << Antara::Core::Get_Instance().Get_status();

    return 0;
}