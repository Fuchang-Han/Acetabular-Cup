#ifndef __GLOBAL__H__
#define __GLOBAL__H__

// VTK
#include "settings.h"
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkActor.h>

// C/C++
#include <vector>
#include <string>
#include <ctime>
#include <tuple>

#undef M_PI
#define M_PI 3.141592653589793f

#define EPSILON 1e-5

typedef vtkSmartPointer<vtkPoints> Points;
typedef std::vector<Points> VecPoints;

//Array Funcs
void SetArray3(double a[3], double x, double y, double z);
void ArrayAdd(const double a[3], const double b[3], double c[3]);  //数组加
void ArrayMinus(const double a[3], const double b[3], double c[3]);  //数组减
void ArrayMulti(const double a, const double b[3], double c[3]);  //数乘
bool ArrayNormalized(double a[3]);   //归一化
double ArrayComuptingDistance(const double a[3], const double b[3]);

// Debug Funcs
float GetLocalSecond(clock_t &lastTime);
void PrintAllPoint(const Points &points);

#endif

