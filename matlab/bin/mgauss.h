#ifndef MGAUSS_H__
#define MGAUSS_H__

#if defined (_WIN32)
#if defined (GAUSS_API_EXPORTS)
#define GAUSS_API __declspec (dllexport)
#else
#define GAUSS_API __declspec (dllimport)
#endif
#else
#define GAUSS_API
#endif

GAUSS_API
void gauss(double** a, double* y, double* x, int n);

GAUSS_API
void Foo(int r, int c, double** mat);

#endif // MGAUSS_H__