
#include <iostream>
#include <errno.h>
#include <cstring>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include "antara_udp.h"

struct sockaddr_storage Addr;
struct addrinfo *p;

int antara_udp_server_init(const char *PORT, const char *IPAddressOrHostName)
{
	struct addrinfo hints, *res;
	int sockfd, status;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	status = getaddrinfo(IPAddressOrHostName, PORT, &hints, &res);

	if (status != 0)
	{
		printf("getaddrinfo() : %s\n", gai_strerror(status));
		return 1;
	}

	printf("getaddrinfo success.\n");

	for (p = res; p != NULL; p = p->ai_next)
	{
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

		if (sockfd < 0)
		{
			perror("socket() ");
			return 1;
		}
		else
		{
			printf("Socket has been created.\n");
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) < 0)
		{
			perror("bind() ");
			close(sockfd);
			return 1;
		}
		else
		{
			printf("Socket binded.\n");
		}
	}

	freeaddrinfo(res);

	return sockfd;
}

int antara_udp_client_init(const char *PORT, const char *IPAddressOrHostName)
{

	struct addrinfo hints, *res, *p;
	int sockfd, status;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	status = getaddrinfo(IPAddressOrHostName, PORT, &hints, &res);

	if (status != 0)
	{
		printf("getaddrinfo() : %s\n", gai_strerror(status));
		return 1;
	}

	printf("getaddrinfo success.\n");

	for (p = res; p != NULL; p = p->ai_next)
	{
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

		if (sockfd < 0)
		{
			perror("socket() ");
			return 1;
		}
		else
		{
			printf("Socket has been created.\n");
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) < 0)
		{
			perror("connect() : ");
			close(sockfd);
			return 1;
		}

		printf("Socket connected.\n");
	}

	freeaddrinfo(res);

	return sockfd;
}

int antara_udp_transmit(const int fd, const unsigned char *messageOut, const std::uint16_t msgLen)
{
	if (sendto(fd, messageOut, msgLen, 0, p->ai_addr, p->ai_addrlen) < 0)
	{
		perror("sendto() ");
		return 1;
	}

	printf("Done Transmit.\n");

	return 0;
}

int antara_udp_recv(const int fd, unsigned char *messageIn, const std::uint16_t msgLen)
{

	socklen_t len;
	len = sizeof(Addr);

	if (recvfrom(fd, messageIn, msgLen, 0, (struct sockaddr *)&Addr, &len) < 0)
	{
		perror("recvfrom() ");
		return 1;
	}

	printf("Received.\n");

	return 0;
}
