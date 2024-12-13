
#include <cstdint>

int antara_serial_init(const char *PORT);

void antara_serial_transmit(const int serialPort, const char *messageTransmit, const std::uint16_t messageLen);

void antara_serial_recv(const int serialPort, char *buffer, const std::uint16_t bufferLen);