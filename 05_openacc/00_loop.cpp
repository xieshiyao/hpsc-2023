#include <openacc.h>

#include <cstdio>

int main()
{
#pragma acc parallel loop
	for (int i = 0; i < 8; i++)
	{
		printf("%d: %d\n", __pgi_vectoridx(), i);
	}
}
