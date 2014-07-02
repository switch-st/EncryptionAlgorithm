#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void ShowData(unsigned char* p, unsigned long len)
{
	int i;
	for (i = 0; i < len; ++i) {
		fprintf(stderr, "%2.2x", p[i]);
	}   
	printf("\n");
}

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf("Usage...\n");
		return 1;
	}
	char p[1024];
	int fd = open(argv[1], O_RDWR);

	int len = read(fd, p, 1024);

	ShowData(p, len);

}

