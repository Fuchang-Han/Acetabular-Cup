#pragma once
#include "Global.h"

// Eigen
#include <Eigen/Dense>

// -------------------------------------------------------
// Mesh

class XMesh
{
public:
	XMesh() { }
	XMesh(vtkPolyData *polydata);
	XMesh(const std::string &filename) { Read(filename); }
public:

	// Seter/Geter Funcs
	vtkSmartPointer<vtkPolyData> GetMeshPolyData() { return PolyData; }

	// 加载和预处理
	bool Read(const std::string &filename);
	void UnitCentralization();   // 物体坐标转换到[-1, 1]


	// Converting
	Eigen::MatrixXf getEigenVerticesMatrix(vtkPolyData *polyData);

private:

private:
	vtkSmartPointer<vtkPolyData> PolyData;

	//double* box;  //包围盒
	double OriginCenter[3];   // 原质心
	double Scale;    //模型缩放因子
};


