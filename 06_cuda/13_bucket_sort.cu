#include <cuda_runtime.h>

#include <cstdio>
#include <cstdlib>
#include <vector>

__device__ void scan(int* a, int* b, int N)
{
	int i = threadIdx.x;
	for (int j = 1; j < N; j <<= 1)
	{
		b[i] = a[i];
		__syncthreads();
		if (i >= j)
			a[i] += b[i - j];
		__syncthreads();
	}
}

__global__ void bucket_sort(int* key, int n, int range)
{
	int i = threadIdx.x;
	extern __shared__ int s[];
	int* bucket = s;
	int* offset = &s[n];
	int* temp = &s[2 * n];

	atomicAdd(&bucket[key[i]], 1);
	__syncthreads();
	if (i >= 1)
		offset[i] = bucket[i - 1];
	__syncthreads();
	scan(offset, temp, n);
	if (i < range)
	{
		int j = offset[i];
		for (; bucket[i] > 0; bucket[i]--)
			key[j++] = i;
	}
}

int main()
{
	int n = 50;
	int range = 5;
	int* key;
	cudaMallocManaged(&key, n * sizeof(int));
	for (int i = 0; i < n; i++)
	{
		key[i] = rand() % range;
		printf("%d ", key[i]);
	}
	printf("\n");

	bucket_sort<<<1, n, 3 * n * sizeof(int)>>>(key, n, range);
	cudaDeviceSynchronize();

	for (int i = 0; i < n; i++)
		printf("%d ", key[i]);
	printf("\n");
	cudaFree(key);
}
