#include "Global.h"
#include<vtkPolyDataMapper.h>
#include<vtkProperty.h>
#include<vtkSphereSource.h>

using namespace std;


//Array Funcs
void SetArray3(double a[3], double x, double y, double z)
{
	a[0] = x;	a[1] = y;	a[2] = z;
}


void ArrayAdd(const double a[3], const double b[3], double c[3])
{
	for (int i = 0; i < 3; ++i)
		c[i] = a[i] + b[i];
}

void ArrayMinus(const double a[3], const double b[3], double c[3])
{
	for (int i = 0; i < 3; ++i)
		c[i] = a[i] - b[i];
}

void ArrayMulti(const double a, const double b[3], double c[3])
{
	for (int i = 0; i < 3; ++i)
		c[i] = a * b[i];
}

bool ArrayNormalized(double a[3])
{
	double m = sqrt(pow(a[0], 2) + pow(a[1], 2) + pow(a[2], 2));
	if (m < EPSILON) return false;	// 判断是否除零

	double length = 1.0 / m;
	ArrayMulti(length, a, a);
	return true;
}

double ArrayComuptingDistance(const double a[3], const double b[3])
{
	double x, y, z;
	x = (a[0] - b[0]) * (a[0] - b[0]);
	y = (a[1] - b[1]) * (a[1] - b[1]);
	z = (a[2] - b[2]) * (a[2] - b[2]);
	return sqrt(x + y + z);
}

// Debug Funcs
float GetLocalSecond(clock_t &lastTime)    //传入开始的时间，计算耗时
{
	clock_t tmp = lastTime;
	lastTime = clock();
	return 0.001f * (lastTime - tmp);
}

void PrintAllPoint(const Points &points)
{
	cout << "\n- Points:" << points->GetNumberOfPoints() << endl;
	for (int i = 0; i < points->GetNumberOfPoints(); i++)
	{
		double p[3];
		points->GetPoint(i, p);
		cout << p[0] << " " << p[1] << " " << p[2] << endl;
	}
}
