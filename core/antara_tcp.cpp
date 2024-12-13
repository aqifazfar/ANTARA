
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

#include "ANTARA/Antara.h"
#include "antara_tcp.h"

#define SA struct sockaddr

struct sockaddr_in Addr;

int antara_tcp_server_init(const char *PORT, const char *IPAddressOrHostName)
{

	struct addrinfo hints, *res, *p;
	int sockfd, newSockfd, status;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	status = getaddrinfo(IPAddressOrHostName, PORT, &hints, &res);

	if (status != 0)
	{
		printf("getaddrinfo() : %s\n", gai_strerror(status));
		return 1;
	}

	printf("getaddrinfo success.\n");

	switch (res->ai_addr->sa_family)
	{
		char s[16];
	case AF_INET:
	{
		struct sockaddr_in *addr_in = (struct sockaddr_in *)res->ai_addr;
		inet_ntop(AF_INET, &(addr_in->sin_addr), s, INET_ADDRSTRLEN);
		printf("IP Address : %s\n", s);
	}
	break;

	default:
	{

		struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)res->ai_addr;
		inet_ntop(AF_INET6, &(addr_in6->sin6_addr), s, INET6_ADDRSTRLEN);
		printf("IP Address : %s\n", s);
		break;
	}
	}

	for (p = res; p != NULL; p = p->ai_next)
	{
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

		if (sockfd < 0)
		{
			perror("socket() ");
			status = 1;
			break;
		}
		else
		{
			printf("Socket has been created.\n");
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) < 0)
		{
			perror("bind() ");
			close(sockfd);
			status = 1;
			break;
		}
		else
		{
			printf("Socket binded.\n");
		}
	}

	freeaddrinfo(res);

	if (status > 0)
	{
		return 1;
	}

	if (listen(sockfd, NUM_VEHICLE) < 0)
	{
		perror("listen() : ");
		close(sockfd);
		return 1;
	}
	printf("Listening..\n");

	socklen_t len = sizeof(Addr);

	if ((newSockfd = accept(sockfd, (SA *)&Addr, &len)) < 0)
	{
		perror("accept() : ");
		close(sockfd);
		return 1;
	}

	printf("New connection accepted.\n");

	return newSockfd;
}

int antara_tcp_client_init(const char *PORT, const char *IPAddressOrHostName)
{

	struct addrinfo hints, *res, *p;
	int sockfd, status;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

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
			perror("connect() ");
			close(sockfd);
			return 1;
		}
		printf("Socket connected.\n");
	}
	freeaddrinfo(res);

	return sockfd;
}

int antara_tcp_transmit(const int fd, const std::uint8_t *messageOut, const std::uint16_t msgLen)
{

	if (send(fd, messageOut, msgLen, 0) < 0)
	{
		perror("send ");
		return 1;
	}
	printf("Done transmit.\n");

	return 0;
}
int antara_tcp_recv(const int fd, std::uint8_t *messageIn, const std::uint16_t msgLen)
{

	if (recv(fd, messageIn, msgLen, 0) < 0)
	{
		perror("recv ");
		return 1;
	}
	printf("received.\n");

	return 0;
}