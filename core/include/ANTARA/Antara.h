#pragma once

#include <cstdint>

//////////////////// USER DEFINED VARIABLE ////////////////////
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 320
#endif

#ifndef NUM_VEHICLE
#define NUM_VEHICLE 1
#endif

#define TCP

//////////////////// -------------------- ////////////////////

enum Side
{

	VEHICLE,
	GCS
};
typedef struct AntaraAddress
{
	std::uint8_t GroupID{0};
	std::uint8_t SystemID{0};
	std::uint8_t NodeID{0};
	std::uint8_t ComponentID{0};
} AntaraAddress_t;

typedef struct AntaraPayload
{
	std::uint8_t MessageID{0};
	std::uint8_t Length{0};
	std::uint8_t *Data{0};
} AntaraPayload_t;

typedef struct AntaraMessage
{
	std::uint8_t ControlFlags{0};
	AntaraAddress_t Add;
	std::uint8_t SequenceNumber{0};
	AntaraPayload_t Payload;
	std::uint8_t *Signature{};
} AntaraMessage_t;

enum ControlFlags
{
	REAL_TIME_MODE = 0,
	ACK_NACK_REQUEST = 1,
	NACK = 2,
	ACK = 3,
	MULTICAST_MODE = 4,
	RELIABLE_MODE = 8,
	BROADCAST_MODE = 12,
	FINISH = 48,
	PRIORITY = 64,
};

namespace antara
{
	int Antara_Init(const std::uint8_t side, const char *PORT, const char *IPAddress);

	void Antara_TX(const int fd, const AntaraMessage_t *msgIn);

	void Antara_RX(const int fd, const std::uint8_t mode, const AntaraAddress_t sysAdd, const AntaraAddress_t recvAdd, AntaraPayload_t *payloadOut);
} // namespace antara