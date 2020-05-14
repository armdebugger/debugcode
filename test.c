#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <execinfo.h>

void FuncA(void)
{
	int i = 0;

	for(; i < 1; ++i)
	{
		char *c = (char*)malloc(sizeof(char));

		if(NULL == c)
		{
			fprintf(stderr, "malloc fails\n");
		}
		else
		{
			fprintf(stderr, "malloc ok\n");
		}
	}
}

void Func_Tst(void)
{
	FuncA();
}

int main()
{
	fprintf(stderr, "main()=%p\n", main);
	fprintf(stderr, "Func_Tst()=%p\n", Func_Tst);
	Func_Tst();
	fprintf(stderr, "main()=%p\n", main);
	return 0;
}
