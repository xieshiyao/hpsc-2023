#include <immintrin.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

float reduce(__m256 avec)
{
	__m256 bvec = _mm256_permute2f128_ps(avec, avec, 1);
	bvec = _mm256_add_ps(bvec, avec);
	bvec = _mm256_hadd_ps(bvec, bvec);
	bvec = _mm256_hadd_ps(bvec, bvec);
	float a[8];
	_mm256_store_ps(a, bvec);
	return a[0];
}

int main()
{
	const int N = 8;
	float x[N], y[N], m[N], fx[N], fy[N];
	for (int i = 0; i < N; i++)
	{
		x[i] = drand48();
		y[i] = drand48();
		m[i] = drand48();
		fx[i] = fy[i] = 0;
	}
	__m256 xavc = _mm256_load_ps(x);
	__m256 yavc = _mm256_load_ps(y);
	__m256 mavc = _mm256_load_ps(m);
	float idx[] = {0, 1, 2, 3, 4, 5, 6, 7};
	__m256 idxavc = _mm256_load_ps(idx);
	for (int i = 0; i < N; i++)
	{
		__m256 allx = _mm256_set1_ps(x[i]);
		__m256 ally = _mm256_set1_ps(y[i]);
		__m256 alli = _mm256_set1_ps(i);
		__m256 mask = _mm256_cmp_ps(idxavc, alli, _CMP_NEQ_OQ);
		__m256 three = _mm256_set1_ps(3.);
		__m256 zero = _mm256_set1_ps(0.);

		__m256 rxavx = _mm256_sub_ps(xavc, allx);
		__m256 ryavx = _mm256_sub_ps(yavc, ally);
		__m256 temp = _mm256_mul_ps(
			_mm256_pow_ps(_mm256_rcp_ps(_mm256_hypot_ps(rxavx, ryavx)), three),
			mavc);
		__m256 fxavc = _mm256_mul_ps(temp, rxavx);
		__m256 fyavc = _mm256_mul_ps(temp, ryavx);
		fxavc = _mm256_blendv_ps(zero, fxavc, mask);
		fyavc = _mm256_blendv_ps(zero, fyavc, mask);
		fx[i] = reduce(fxavc);
		fy[i] = reduce(fyavc);
		printf("%d %g %g\n", i, fx[i], fy[i]);
	}
}
