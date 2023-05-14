#include "UDPSocket.h"
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <string>
#include <cmath>

//using namespace std;
std::string xorCipher(const std::string& text, const std::string& key) {
    std::string encryptedText;
    for (size_t i = 0; i < text.length(); ++i) {
        encryptedText += text[i] ^ key[i % key.length()];
    }
    return encryptedText;
}

void printHex(const std::string& text) {
    for (char c : text) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
    }
    std::cout << std::endl;
}

int main() {
	//	SOCKET s;
	struct sockaddr_in si_other;
	int slen = sizeof(si_other);

	//struct sockaddr_in si_other1;
	//int slen1 = sizeof(si_other1);

	char buf[BUFLEN];
	char message[BUFLEN];

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


	unsigned short srv_port = 0;
	char srv_ip_addr[40];
	memset(srv_ip_addr, 0, 40);


	//create socket
	UDPSocket client_sock;

	//setup address structure
	memset((char*)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;

	if (1 == argc)
	{
		si_other.sin_port = htons(PORT);
		si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);
		printf("1: Server - addr=%s , port=%d\n", SERVER, PORT);
	}
	else if (2 == argc)
	{
		si_other.sin_port = htons(atoi(argv[1]));
		si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);
		printf("2: argv[0]: Server - addr=%s , port=%d\n", SERVER, atoi(argv[1]));
	}
	else
	{
		si_other.sin_port = htons(atoi(argv[2]));
		si_other.sin_addr.S_un.S_addr = inet_addr(argv[1]);
		printf("3: Server - addr=%s , port=%d\n", argv[1], atoi(argv[2]));
	}

	while (1)
	{
		memset(buf, '\0', BUFLEN);
		memset(message, '\0', BUFLEN);

		printf("Enter message: ");
		gets_s(message, BUFLEN);
		std::string decod_string(message);
		decod_string = decryptedText(decod_string);
		decod_string = encryptedText(decod_string);
		strcpy_s(message, decod_string.c_str());
		client_sock.SendDatagram(message, (int)strlen(message), (struct sockaddr*)&si_other, slen);
		memset(message, '\0', BUFLEN);

		client_sock.RecvDatagram(buf, BUFLEN, (struct sockaddr*)&si_other, &slen);
		std::string ncode_string(buf);
		ncode_string = decryptedText(ncode_string);
		ncode_string = encryptedText(ncode_string);
		strcpy_s(buf, ncode_string.c_str());
		printf("Answer: %s\n", buf);
		memset(buf, '\0', BUFLEN);
	}
	return 0;
}