#pragma once

#include <iostream>
#include <cstring>
#include <cstdint>

static const unsigned char b64EncTable[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const std::uint8_t b64DecTable[] = {
	80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
	80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
	80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 62, 80, 80, 80, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 80, 80, 80, 64, 80, 80,
	80, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 80, 80, 80, 80, 80,
	80, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 80, 80, 80, 80, 80};

void b64_encode(std::uint8_t *dest, const std::uint8_t *src)
{
	std::uint16_t i, j, len;

	len = 4 * (std::strlen((const char *)src) / 3) + 1;
	j = 0;

	for (i = 0; i < len; i += 3)
	{
		dest[j++] = b64EncTable[(src[i] >> 2)];

		dest[j++] = b64EncTable[((src[i] & 0x03) << 4) | ((src[i + 1] & 0xF0) >> 4)];

		if ((i + 1) < len)
		{
			dest[j++] = b64EncTable[((src[i + 1] & 0x0F) << 2) | ((src[i + 2] & 0xC0) >> 6)];
		}
		else
		{
			dest[j++] = '=';
		}

		if ((i + 2) < len)
		{
			dest[j++] = b64EncTable[(src[i + 2] & 0x3F)];
		}
		else
		{
			dest[j++] = '=';
		}
	}
}

void b64_decode(std::uint8_t *dest, const std::uint8_t *src)
{
	std::uint16_t i, j, len;

	len = std::strlen((const char *)src);

	for (i = 0; i < len; i++)
	{
		if (!((src[i] >= '0' && src[i] <= '9') ||
			  (src[i] >= 'A' && src[i] <= 'Z') ||
			  (src[i] >= 'a' && src[i] <= 'z') ||
			  (src[i] == '+' || src[i] == '/' || src[i] == '=')))
		{
			printf("Base64 not invalid characters.\n");
			return;
		}
	}

	j = 0;

	for (i = 0; i < len; i += 4)
	{
		if (src[i + 3] == '=')
		{
			if (src[i + 2] == '=')
			{
				dest[j++] = (std::uint8_t)((b64DecTable[src[i]] << 2) | ((b64DecTable[src[i + 1]] & 0xF0) >> 4));
			}
			else
			{
				dest[j++] = (std::uint8_t)(((b64DecTable[src[i + 1]] & 0x0F) << 4) | ((b64DecTable[src[i + 2]] & 0x3C) >> 2));

				dest[j++] = (std::uint8_t)((b64DecTable[src[i]] << 2) | ((b64DecTable[src[i + 1]] & 0xF0) >> 4));
			}

			dest[j++] = (std::uint8_t)(((b64DecTable[src[i + 2]] & 0x03) << 6) | (b64DecTable[src[i + 3]] & 0x3F));
		}
		else
		{
			dest[j++] = (std::uint8_t)((b64DecTable[src[i]] << 2) | ((b64DecTable[src[i + 1]] & 0xF0) >> 4));

			dest[j++] = (std::uint8_t)(((b64DecTable[src[i + 1]] & 0x0F) << 4) | ((b64DecTable[src[i + 2]] & 0x3C) >> 2));

			dest[j++] = (std::uint8_t)(((b64DecTable[src[i + 2]] & 0x03) << 6) | (b64DecTable[src[i + 3]] & 0x3F));
		}
	}
}