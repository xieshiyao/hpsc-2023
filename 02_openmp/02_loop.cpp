#include <omp.h>

#include <cstdio>

int main()
{
	omp_set_num_threads(6);
	int a = 0, b = 0;
#pragma omp parallel
	for (int i = 0; i < 800; i++)
	{
		printf("i: %d, id: %d\n", i, omp_get_thread_num());
#pragma omp atomic update
		a++;
#pragma omp for
		for (int j = 10; j <= 14; j++)
		{
#pragma omp atomic update
			b++;
		}
	}
	printf("a: %d\nb: %d\n", a, b);
}
