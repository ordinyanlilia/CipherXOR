#include <stdio.h>
#include "UDPServer.h"
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <string>
#include <cmath>

#ifndef _WIN32
using SOCKET = int
#define WSAGetLastError() 1
#else
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#endif

//#define SERVER "127.0.0.1"	//ip address of udp server
#define BUFLEN 	1024		    //Max length of buffer
#define PORT    8888			//The port on which to listen for incoming data

int main() {
	struct sockaddr_in si_other;
	//struct sockaddr_in si_other1;
	unsigned short srvport;
	int slen;
	char buf[BUFLEN];
	char msg[BUFLEN];

	std::string xorCipher(const std::string & text, const std::string & key) {
		std::string encryptedText;
		for (size_t i = 0; i < text.length(); ++i) {
			encryptedText += text[i] ^ key[i % key.length()];
		}
		return encryptedText;
	}

	void printHex(const std::string & text) {
		for (char c : text) {
			std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
		}
		std::cout << std::endl;
	}

	int main() {
		struct sockaddr_in si_other;
		//struct sockaddr_in si_other1;
		unsigned short srvport;
		int slen;
		char buf[BUFLEN];
		char msg[BUFLEN];
		std::string text;
		std::cout << "Enter the text: ";
		std::getline(std::cin, text);

		std::string key;
		std::cout << "Enter the key: ";
		std::getline(std::cin, key);

		// Encryption
		std::string encryptedText = xorCipher(text, key);
		std::cout << "Encrypted Text (Hex): ";
		printHex(encryptedText);

		// Decryption
		std::string decryptedText = xorCipher(encryptedText, key);
		std::cout << "Decrypted Text: " << decryptedText << std::endl;

		srvport = (1 == argc) ? PORT : atoi(argv[1]);

		UDPServer server(srvport);
		slen = sizeof(si_other);

		while (1)
		{
			memset(msg, '\0', BUFLEN);
			memset(buf, '\0', BUFLEN);

			printf("Waiting for data...   ");
			server.RecvDatagram(buf, BUFLEN, (struct sockaddr*)&si_other, &slen);
			std::string ncode_string(buf);
			ncode_string = decryptedText(ncode_string);
			ncode_string = encryptedText(ncode_string);
			strcpy_s(buf, ncode_string.c_str());
			printf("%s ", buf);
			memset(buf, '\0', BUFLEN);

			printf("\nAnswer : ");
			gets_s(msg, BUFLEN);
			std::string decod_string(msg);
			decod_string = decryptedText(decod_string);
			decod_string = encryptedText(decod_string);
			strcpy_s(msg, decod_string.c_str());
			server.SendDatagram(msg, (int)strlen(msg), (struct sockaddr*)&si_other, slen);
			memset(msg, '\0', BUFLEN);
		}
		return 0;

	}
}