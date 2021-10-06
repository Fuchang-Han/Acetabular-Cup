#include "XGeometry.h"

#include <cmath>

//VTK
#include <vtkOBJReader.h>
#include <vtkSTLReader.h>
#include <vtkSTLReader.h>
#include <vtkPlaneSource.h>
#include <vtkOBBTree.h>
#include <vtkSphereSource.h>
#include <vtkProperty.h>

using namespace std;
using namespace Eigen;
//#define DEBUG_RENDER


XMesh::XMesh(vtkPolyData *polydata)
{
	this->PolyData = polydata;

	PolyData->GetCenter(OriginCenter);//质心
	Scale = 1.0;
}

// -------------------------------------------------------------------
// Seter/Geter Funcs
// -------------------------------------------------------------------


bool XMesh::Read(const std::string & filename)
{
	//1.导入obj
	//AGLM::showOBJ(reader.getObjData());    //obj模型显示
	string suffix = filename.substr(filename.size() - 3, 3);
	transform(suffix.begin(), suffix.end(), suffix.begin(), tolower);
	if (suffix == "obj") {
		vtkNew<vtkOBJReader> reader;
		reader->SetFileName(filename.c_str());
		reader->Update();
		PolyData = reader->GetOutput();
	}
	else if (suffix == "stl") {
		vtkNew<vtkSTLReader> reader;
		reader->SetFileName(filename.c_str());
		reader->Update();
		PolyData = reader->GetOutput();

	}
	else return false;

	PolyData->GetCenter(OriginCenter);//质心
	Scale = 1.0;

	return true;
}


// ---------------------------------------------------------------------
// Algorithm
// ---------------------------------------------------------------------

void XMesh::UnitCentralization()   //将物体坐标转换到[-1, 1]
{
	double *box = PolyData->GetBounds();  //包围盒

	Scale = max({ box[1] - box[0], box[3] - box[2], box[5] - box[4] });  //模型缩放因子

	//修改limbPolyData
	double pnt[3];
	vtkPoints *pnts = PolyData->GetPoints();     //指针*pnts取limbPolyData的地址->*pnts所指向的内容改变，limbPolyData也会变
	int cnt = pnts->GetNumberOfPoints();
	for (int i = 0; i < cnt; ++i) {
		pnts->GetPoint(i, pnt);
		for (int j = 0; j < 3; ++j)
			pnt[j] = (pnt[j] - OriginCenter[j]) / Scale;
		pnts->SetPoint(i, pnt);
	}
	pnts->Modified();
}

// 将VTK顶点数据转换为Eigen矩阵数据(顶点矩阵, n行3列)
Eigen::MatrixXf XMesh::getEigenVerticesMatrix(vtkPolyData *polyData)
{
	int numVerts = polyData->GetNumberOfVerts();

	MatrixXf V;
	V.resize(numVerts, 3);
	double pnt[3];
	vtkPoints *pnts = PolyData->GetPoints();     //指针*pnts取limbPolyData的地址->*pnts所指向的内容改变，limbPolyData也会变
	for (int i = 0; i < numVerts; i++)
	{
		pnts->GetPoint(i, pnt);
		for(int k = 0; k < 3; k++) V(i, k) = pnt[k];
	}
	return V;
}
