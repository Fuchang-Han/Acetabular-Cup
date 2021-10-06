#ifndef __XREADER_H__

#define __XRENDER_H__

#include "Global.h"

#include <vtkRenderWindowInteractor.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkActor2D.h>


class XRender {
public:
	XRender(int w = 1200, int h = 800) { Init(w, h); }
	~XRender();

	void Init(int w, int h);
	void AddAllActors();
	void Rendering();
	void OffScreenRendering(const std::string &filename);

public:
	// Add Base Render Object
	void AddActor(vtkActor *a) { Actors.push_back(a); }
	void AddActor2D(vtkActor2D *a) { Actor2Ds.push_back(a); }
	void AddMapper(vtkMapper *m);
	void AddPolyData(vtkPolyData *pd);
	// Add Advanced Render Object
	void AddPoints(vtkPoints *p, double sphereScale = 0.008);

	vtkActor* GetLastActor() { return Actors[Actors.size() - 1]; }   //返回最后一个actor
	vtkProperty* GetLastProperty() { return GetLastActor()->GetProperty(); }

	// Geter or Seter
	void SetShowAxes(bool isShow) { ShowAxes = isShow; }
	void SetBackgroundColor(double r, double g, double b) { SetArray3(BackgroundColor, r, g, b); Renderer->SetBackground(BackgroundColor); }

private:

private:
	// Render Window
	int Width, Height;
	double BackgroundColor[3] = { .3, .6, .3 };
	const double AxisLen = 1.0;

	vtkSmartPointer<vtkRenderer> Renderer;
	vtkSmartPointer<vtkRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderWindowInteractor> RenderWindowInteractor;

	// Render Data
	std::vector<vtkSmartPointer<vtkActor> > Actors;
	std::vector<vtkSmartPointer<vtkActor2D> > Actor2Ds;
	bool ShowAxes = true;
};


#endif // !_XREADER_H_

