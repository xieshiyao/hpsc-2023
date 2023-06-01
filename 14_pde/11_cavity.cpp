#include <cmath>
#include <iostream>
#include <vector>
using namespace std;
using matrix = vector<vector<float>>;

int main()
{
	int nx = 41;
	int ny = 41;
	int nt = 500;
	int nit = 50;
	float dx = 2. / (nx - 1);
	float dy = 2. / (ny - 1);
	float dt = .01;
	float rho = 1.;
	float nu = .02;

	vector<float> x(nx), y(ny);
	for (int i = 0; i < nx; i++)
		x[i] = i * dx;
	for (int i = 0; i < ny; i++)
		y[i] = i * dy;

	matrix u(ny, vector<float>(nx));
	matrix v(ny, vector<float>(nx));
	matrix p(ny, vector<float>(nx));
	matrix b(ny, vector<float>(nx));
	// X, Y = np.meshgrid(x, y)

	for (int n = 0; n < nt; n++)
	{
		for (int j = 1; j < ny - 1; j++)
			for (int i = 1; i < nx - 1; i++)
			{
				b[j][i]
					= rho
					  * (1 / dt
							 * ((u[j][i + 1] - u[j][i - 1]) / (2 * dx)
								+ (v[j + 1][i] - v[j - 1][i]) / (2 * dy))
						 - pow(((u[j][i + 1] - u[j][i - 1]) / (2 * dx)), 2)
						 - 2
							   * ((u[j + 1][i] - u[j - 1][i]) / (2 * dy)
								  * (v[j][i + 1] - v[j][i - 1]) / (2 * dx))
						 - pow(((v[j + 1][i] - v[j - 1][i]) / (2 * dy)), 2));
			}
		for (int it; it < nit; it++)
		{
			matrix pn = p;
			for (int j = 1; j < ny - 1; j++)
				for (int i = 1; i < nx - 1; i++)
					p[j][i] = (pow(dy, 2) * (pn[j][i + 1] + pn[j][i - 1])
							   + pow(dx, 2) * (pn[j + 1][i] + pn[j - 1][i])
							   - b[j][i] * pow(dx, 2) * pow(dy, 2))
							  / (2 * (pow(dx, 2) + pow(dy, 2)));
			for (int i = 0; i < ny; i++)
			{
				p[i][nx - 1] = p[i][nx - 2];
				p[i][0] = p[i][1];
			}
			for (int j = 0; j < nx; j++)
			{
				p[0][j] = p[1][j];
				p[ny - 1][j] = 0;
			}
		}
		matrix un = u;
		matrix vn = v;
#pragma omp parallel for
		for (int j = 1; j < ny - 1; j++)
			for (int i = 1; i < nx - 1; i++)
			{
				u[j][i] = un[j][i]
						  - un[j][i] * dt / dx * (un[j][i] - un[j][i - 1])
						  - un[j][i] * dt / dy * (un[j][i] - un[j - 1][i])
						  - dt / (2 * rho * dx) * (p[j][i + 1] - p[j][i - 1])
						  + nu * dt / pow(dx, 2)
								* (un[j][i + 1] - 2 * un[j][i] + un[j][i - 1])
						  + nu * dt / pow(dy, 2)
								* (un[j + 1][i] - 2 * un[j][i] + un[j - 1][i]);
				v[j][i] = vn[j][i]
						  - vn[j][i] * dt / dx * (vn[j][i] - vn[j][i - 1])
						  - vn[j][i] * dt / dy * (vn[j][i] - vn[j - 1][i])
						  - dt / (2 * rho * dx) * (p[j + 1][i] - p[j - 1][i])
						  + nu * dt / pow(dx, 2)
								* (vn[j][i + 1] - 2 * vn[j][i] + vn[j][i - 1])
						  + nu * dt / pow(dy, 2)
								* (vn[j + 1][i] - 2 * vn[j][i] + vn[j - 1][i]);
			}
		for (int i = 0; i < ny; i++)
		{
			u[i][0] = u[i][nx - 1] = 0;
			v[i][0] = v[i][nx - 1] = 0;
		}
		for (int j = 0; j < nx; j++)
		{
			u[0][j] = u[ny - 1][j] = 0;
			v[0][j] = v[ny - 1][j] = 0;
		}
		// plt.contourf(X, Y, p, alpha=0.5, cmap=plt.cm.coolwarm)
		// plt.quiver(X[::2, ::2], Y[::2, ::2], u[::2, ::2], v[::2, ::2])
		// plt.pause(.01)
		// plt.clf()
	}
	// plt.show()
}
