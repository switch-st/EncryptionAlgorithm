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

	EncryptShift shift;
	shift.SetData((const unsigned char*)"abc", 3);
	shift.Shift(3, DIR_SHIFT_LEFT);
	shift.GetData(&p, &len);

	ShowData(p, len);

	return 0;
}
