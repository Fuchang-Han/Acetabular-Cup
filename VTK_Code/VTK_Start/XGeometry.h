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

	// ���غ�Ԥ����
	bool Read(const std::string &filename);
	void UnitCentralization();   // ��������ת����[-1, 1]


	// Converting
	Eigen::MatrixXf getEigenVerticesMatrix(vtkPolyData *polyData);

private:

private:
	vtkSmartPointer<vtkPolyData> PolyData;

	//double* box;  //��Χ��
	double OriginCenter[3];   // ԭ����
	double Scale;    //ģ����������
};


