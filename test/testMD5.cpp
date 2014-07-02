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

	if (argc == 1) {
		cerr << "Usage..." << endl;
		return 1;
	}

	/*
	src = (unsigned char*)malloc(strlen(argv[1]) + 2);
	memcpy(src, argv[1], strlen(argv[1]));
	src[strlen(argv[1])] = '\n';
	src[strlen(argv[1]) + 1] = '\0';

	EncryptMD5 md5(src, strlen((const char*)src));

	p = md5.GetResult();
	if (p == NULL) {
		cerr << "GetResult error" << endl;
		return 1;
	}

	ShowData(p, 16);
	md5.Write2File("md5.log");

	free(src);
	*/

	EncryptMD5 md5;

	md5.SetData(argv[1]);
	p = md5.GetResult();
	ShowData(p, 16);

	return 0;
}
