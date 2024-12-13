#include <cstdint>

int antara_tcp_server_init(const char *PORT, const char *IPAddressOrHostName);

int antara_tcp_client_init(const char *PORT, const char *IPAddressOrHostName);

int antara_tcp_transmit(const int fd, const std::uint8_t *messageOut, const std::uint16_t msgLen);
int antara_tcp_recv(const int fd, std::uint8_t *messageIn, const std::uint16_t msgLen);