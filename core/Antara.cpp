#include <iostream>
#include <cstring>

#include "antara_tcp.h"
#include "antara_udp.h"
#include "antara_serialization.h"
#include "crc32.h"

// #include "base64.h"

#include "ANTARA/Antara.h"

int antara::Antara_Init(const std::uint8_t side, const char *PORT, const char *IPAddress)
{
	int fd = 0;

	// checking byte order
	if (side == VEHICLE)
	{
#if defined(TCP)
		fd = antara_tcp_server_init(PORT, IPAddress);
#elif defined(UDP)
		fd = antara_udp_server_init(PORT, IPAddress);
#else
		printf("Antara_Init() will return 0");
#endif
	}
	else if (side == GCS)
	{

#if defined(TCP)
		fd = antara_tcp_client_init(PORT, IPAddress);
#elif defined(UDP)
		fd = antara_udp_client_init(PORT, IPAddress);
#else
		printf("Antara_Init() will return 0");
#endif
	}
	return fd;
}
void antara::Antara_TX(const int fd, const AntaraMessage_t *msgIn)
{

	std::uint8_t buffer[BUFFER_SIZE];
	std::memset(buffer, 0, BUFFER_SIZE);

	antara_serialization(buffer, *msgIn);

	// b64_encode(buffer, buffer);

	// Test & Integration
	//  for (std::uint8_t i = 0; i < strlen(buffer); i++)
	//  {
	//  	printf("%" PRIu8, buffer[i]);
	//  	printf(" ");
	//  }
	//  printf("\n");

#if defined(TCP)
	antara_tcp_transmit(fd, buffer, std::strlen((const char *)buffer));
#elif defined(UDP)
	antara_udp_transmit(fd, buffer, BUFFER_SIZE);
#else
	printf("Antara_TX() will return an array of serialized data");
#endif
}

void antara::Antara_RX(const int fd, const std::uint8_t mode, const AntaraAddress_t sysAdd, const AntaraAddress_t recvAdd, AntaraPayload_t *payloadOut)
{
	std::uint8_t i;
	AntaraMessage_t msgRespond = {};
	std::uint8_t buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);

#if defined(TCP)
	antara_tcp_recv(fd, buffer, BUFFER_SIZE);
#elif defined(UDP)
	antara_udp_recv(fd, buffer, BUFFER_SIZE);
#else
	printf("None");
#endif

	// b64_decode(buffer2, buffer1);

	// Test & Integration
	// for (i = 0; i < strlen(buffer1) - 4; i++)
	// {
	// 	printf("%" PRIu8, buffer1[i]);
	// 	printf(" ");
	// }

	// Checking Addresses
	switch (mode)
	{
	case MULTICAST_MODE:

		if (buffer[1] != recvAdd.GroupID)
		{
			return;
		}

	case BROADCAST_MODE:

		// Across groups communication, ignore all ID
		break;

	default:
		if ((buffer[1] != recvAdd.GroupID) || (buffer[2] != recvAdd.SystemID))
		{
			// Not in the same group or not the system user wanted
			return;
		}
	}

	// Checking for packet corruption
	if ((crc32(buffer, (std::strlen((const char *)buffer) - 4))) != 0)
	{
		// Reliable Mode will handle the data corruption by requesting the sender to resend the same packet
		if (mode == RELIABLE_MODE)
		{
			msgRespond.Add.GroupID = sysAdd.GroupID;
			msgRespond.Add.SystemID = sysAdd.SystemID;
			msgRespond.Add.NodeID = sysAdd.NodeID;
			msgRespond.Add.ComponentID = sysAdd.ComponentID;
			msgRespond.ControlFlags = (NACK | RELIABLE_MODE);
			Antara_TX(fd, &msgRespond);
			Antara_RX(fd, RELIABLE_MODE, sysAdd, recvAdd, payloadOut);
		}

		// Other mode will ignore the packet that was corrupted
		else
		{
			return;
		}
	}

	// Deserialization
	std::uint8_t buffer2[255];

	for (i = 0; i < buffer[7];
		 i++)
	{
		buffer2[i] = buffer[8 + i];
	}

	payloadOut->MessageID = buffer[6];
	payloadOut->Length = buffer[7];

	memcpy(payloadOut->Data, buffer2, std::strlen((char *)buffer2));
}
