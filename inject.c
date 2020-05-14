

#include <stdio.h>
#include <sys/socket.h>

__attribute__((visibility("default")))
int socket(int family, int type, int protocol)
{
	printf("detect socket call\n");

	return -1;
}

__attribute__((constructor)) void main()
{
	printf("module inject success\n");
}
