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

	PolyData->GetCenter(OriginCenter);//����
	Scale = 1.0;
}

// -------------------------------------------------------------------
// Seter/Geter Funcs
// -------------------------------------------------------------------


bool XMesh::Read(const std::string & filename)
{
	//1.����obj
	//AGLM::showOBJ(reader.getObjData());    //objģ����ʾ
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

	PolyData->GetCenter(OriginCenter);//����
	Scale = 1.0;

	return true;
}


// ---------------------------------------------------------------------
// Algorithm
// ---------------------------------------------------------------------

void XMesh::UnitCentralization()   //����������ת����[-1, 1]
{
	double *box = PolyData->GetBounds();  //��Χ��

	Scale = max({ box[1] - box[0], box[3] - box[2], box[5] - box[4] });  //ģ����������

	//�޸�limbPolyData
	double pnt[3];
	vtkPoints *pnts = PolyData->GetPoints();     //ָ��*pntsȡlimbPolyData�ĵ�ַ->*pnts��ָ������ݸı䣬limbPolyDataҲ���
	int cnt = pnts->GetNumberOfPoints();
	for (int i = 0; i < cnt; ++i) {
		pnts->GetPoint(i, pnt);
		for (int j = 0; j < 3; ++j)
			pnt[j] = (pnt[j] - OriginCenter[j]) / Scale;
		pnts->SetPoint(i, pnt);
	}
	pnts->Modified();
}

// ��VTK��������ת��ΪEigen��������(�������, n��3��)
Eigen::MatrixXf XMesh::getEigenVerticesMatrix(vtkPolyData *polyData)
{
	int numVerts = polyData->GetNumberOfVerts();

	MatrixXf V;
	V.resize(numVerts, 3);
	double pnt[3];
	vtkPoints *pnts = PolyData->GetPoints();     //ָ��*pntsȡlimbPolyData�ĵ�ַ->*pnts��ָ������ݸı䣬limbPolyDataҲ���
	for (int i = 0; i < numVerts; i++)
	{
		pnts->GetPoint(i, pnt);
		for(int k = 0; k < 3; k++) V(i, k) = pnt[k];
	}
	return V;
}
