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
	unsigned char* src;
	unsigned char* p;
	unsigned long len, len1;

	if (argc == 1) {
		cerr << "Usage..." << endl;
		return 1;
	}

	src = (unsigned char*)malloc(strlen(argv[1]) + 2);
	memcpy(src, argv[1], strlen(argv[1]));
	src[strlen(argv[1])] = '\0';
	//src[strlen(argv[1])] = '\n';
	//src[strlen(argv[1]) + 1] = '\0';

	EncryptDES des;
	des.SetPlainText(src, strlen((const char*)src));
	des.SetKey((const unsigned char*)"123", 3);
	//des.SetIvec((const unsigned char*)"1234", 4);
	des.Encrypt();
	des.GetCipherText(&p, &len);

	ShowData(p, len);

	EncryptDES des2;
	des2.SetCipherText(p, len);
	des2.SetKey((const unsigned char*)"123", 3);
	//des2.SetIvec((const unsigned char*)"1234", 4);
	des2.Decrypt();
	des2.GetPlainText(&p, &len);
	printf("len = %d\n", len);

	ShowData(p, len);

	free(src);


	return 0;
}
