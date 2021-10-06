#include"Global.h"

// C++
#include <iostream>
#include <fstream> 
#include <random>
#include <array>

// Our Methods
#include "XRender.h"
#include "XGeometry.h"

// Eigen
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;


// row vector, the first one is largest one (eigen vector)
Matrix3f getPrincipalAxes(const MatrixXf &V)
{
	Matrix3f cov_s = V.transpose() * V;	// covariance matrix of source mesh
	SelfAdjointEigenSolver<Matrix3f> eigensolver(cov_s);
	if (eigensolver.info() != Success) abort();
	Vector3f eig_vals = eigensolver.eigenvalues();				// given in ascending order
	Matrix3f principal_axes = eigensolver.eigenvectors();		// column vector(ascending)
	//cout << "The eigenvalues of cov are:\n" << eig_vals
	//	<< "\nHere's a matrix whose columns are eigenvectors of cov corresponding to these eigenvalues:\n"
	//	<< principal_axes << endl << endl;

	return principal_axes;
}

int main() {

	// ------------------------------------------------------
	// Step 1: Execute Algorithm
	const string folder = "data/";
	const string wFolder = folder + "res/";
	const string modelFilename = folder + "diameter65mm.stl";

	XMesh mesh(modelFilename);
	//mesh.UnitCentralization();

	XRender render;
	render.AddPolyData(mesh.GetMeshPolyData());
	render.SetShowAxes(false);



	render.SetBackgroundColor(0, 0, 0);
	render.GetLastProperty()->SetColor(1, 1, 1);
	render.GetLastActor()->RotateX(-90);	//	 看向Y轴正方向
	//render.Rendering();
	//render.OffScreenRendering();
	//render.GetLastProperty()->FrontfaceCullingOn();
	//render.GetLastProperty()->BackfaceCullingOn();
	render.AddAllActors();

	{
		string picFilename = wFolder + "s_";

		//int stepRA = 1, stepRI = 1;
		int stepRA = 41, stepRI = 21; //注意，产生图像时，这里需要变动

		for (int i = 0; i < stepRA; i++)
		{
			// 前倾角
			//float anteversion = stepRA == 1 ? 10.0f : 10.0f + (20.0f - 10.0f) / (stepRA - 1) * i;
			//float anteversion = stepRA == 1 ? 10.0f : 10.0f + 0.6f * i;
			
			
			float anteversion = 5+0.5*i;
				



			for (int j = 0; j < stepRI; j++)
			{
				// 外展角
				//float inclination = stepRI == 1 ? 35.0f : 35.0f + (55.0f - 35.0f) / (stepRI - 1) * j;
				//float inclination = stepRI == 1 ? 35.0f : 35.0f + (55.0f - 35.0f) / (stepRI - 1) * j;
				float inclination = 50.5 + 0.5*j;



				// Step 1: 旋转后半球体
				render.GetLastActor()->RotateX(-anteversion);
				render.GetLastActor()->RotateY(-inclination);

				// Step 2: 输出模型文件
				 float anteversionInt = anteversion;
				 float inclinationInt = inclination;
				//int anteversionInt = (int)(anteversion + 0.5f);
				//int inclinationInt = (int)(inclination + 0.5f);

				cout << "RA: " << anteversionInt << ", " << "RI:" << inclinationInt << endl;
				string postStr = to_string(anteversionInt) + "_" + to_string(inclinationInt) + ".bmp";
				render.OffScreenRendering(picFilename + postStr);

				render.GetLastActor()->RotateY(inclination);
				render.GetLastActor()->RotateX(anteversion);
			}
		}
	}


	return 0;
}
