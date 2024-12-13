#include <cstdint>

int antara_udp_server_init(const char *PORT, const char *IPAddressOrHostName);

int antara_udp_client_init(const char *PORT, const char *IPAddressOrHostName);

int antara_udp_transmit(const int fd, const unsigned char *messageOut, const std::uint16_t msgLen);
int antara_udp_recv(const int fd, unsigned char *messageIn, const std::uint16_t msgLen);