#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "Encrypt.hpp"

using namespace std;
USING_NAMESPACE_SWITCHTOOL;

void ShowData(unsigned char* p, unsigned long len)
{
	for (int i = 0; i < len; ++i) {
		printf("%2.2x", p[i]);
	}
	printf("\n");
}


int main(int argc, char** argv)
{
	unsigned char* p;
	unsigned long len;
	const char* pData = "Henry Alfred Kissinger is a German-born American writer, political scientist, diplomat, and businessman. A recipient of the Nobel Peace Prize, he served as National Security Advisor and later concurrently as Secretary of State in the administrations of Presidents Richard Nixon and Gerald Ford.";

	p = (unsigned char*)pData;
	cout << "PlainText : " << pData << endl;
	cout << "len : " << strlen(pData) << endl;
	EncryptBase64 b64;
	b64.SetPlainText(p, strlen((char*)p));
	b64.Encode();
	b64.GetCipherText(&p, &len);
	cout << "CipherText : ";
	fflush(stdout);
	write(1, p, len);
	cout << endl << "len : " << len << endl;
	b64.SetCipherText(p, len);
	b64.Decode();
	b64.GetPlainText(&p, &len);
	cout << "PlainText : ";
	fflush(stdout);
	write(1, p, len);
	cout << endl << "len : " << len << endl;

	//ShowData(p, len);

	return 0;
}
