
#include "crc32.h"
#include "antara_serialization.h"

// #include <inttypes.h>

void antara_serialization(std::uint8_t *out, AntaraMessage_t msg)
{
	std::uint8_t i;
	uint32_t crcVal = 0;

	out[0] = msg.ControlFlags;

	out[1] = msg.Add.GroupID;
	out[2] = msg.Add.SystemID;
	out[3] = msg.Add.NodeID;
	out[4] = msg.Add.ComponentID;

	out[5] = ++msg.SequenceNumber;

	out[6] = msg.Payload.MessageID;

	out[7] = msg.Payload.Length;

	for (i = 0; i < msg.Payload.Length; i++)
	{
		out[8 + i] = msg.Payload.Data[i];
	}

	crcVal = crc32(out, (8 + msg.Payload.Length));

	for (i = 0; i <= 3; i++)
	{
		out[8 + msg.Payload.Length + i] = (crcVal >> (8 * (3 - i)) & 0xFF);
	}

	for (i = 0; i <= 3; i++)
	{

		out[12 + msg.Payload.Length + i] = msg.Signature[i];
	}

	/* For Test & Integration

	for (i = 0; i < strlen(out); i++)
	{
		printf("%" PRIu8, out[i]);
		printf(" ");
	}
	printf("\n");

	*/
}
