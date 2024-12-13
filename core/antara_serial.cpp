#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <errno.h>

#include "antara_serial.h"

int antara_serial_init(const char *PORT)
{
	int serialPort = 0;

	serialPort = open(PORT, O_RDWR);

	if (serialPort != 0)
	{
		printf("Error %i from open : %s\n", errno, strerror(errno));
		return 1;
	}

	struct termios srlCon;

	srlCon.c_cflag &= ~(CSIZE | PARENB | CSTOPB | CRTSCTS);

	srlCon.c_cflag |= (CREAD | CLOCAL | CS8);

	srlCon.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHONL | ISIG);

	srlCon.c_iflag &= ~(IXON | IXOFF | IXANY | IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

	srlCon.c_oflag &= ~(OPOST | ONLCR);

	srlCon.c_cc[VMIN] = 0;
	srlCon.c_cc[VTIME] = 10; // 1 second timeout

	cfsetspeed(&srlCon, B115200);

	if (tcsetattr(serialPort, TCSANOW, &srlCon) != 0)
	{
		printf("Error %i from tcsetattr %s\n", errno, strerror(errno));
	}
	return serialPort;
}

void antara_serial_transmit(const int serialPort, const char *messageTransmit, const std::uint16_t messageLen)
{
	if (write(serialPort, messageTransmit, messageLen) != 0)
	{
		printf("Error %i from write %s\n", errno, strerror(errno));

		return;
	}
}

void antara_serial_recv(const int serialPort, char *buffer, const std::uint16_t bufferLen)
{
	if (read(serialPort, buffer, bufferLen) != 0)
	{
		printf("Error %i from read %s\n", errno, strerror(errno));

		return;
	}
}
