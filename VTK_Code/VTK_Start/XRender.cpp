#include "XRender.h"

#include <thread> 
#include <mutex>

#include <vtkPlaneSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkAxesActor.h>
#include <vtkConeSource.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>
#include <vtkGlyph3DMapper.h>

#include <vtkCamera.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include <vtkWindowToImageFilter.h>
#include <vtkBMPWriter.h>

XRender::~XRender()
{
}


void XRender::Init(int w, int h)
{
	Renderer = vtkSmartPointer<vtkRenderer>::New();
	RenderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	// 背景颜色
	Renderer->SetBackground(BackgroundColor);


	// Render and interact
	renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(Renderer);
	renderWindow->SetSize(w, h);
	RenderWindowInteractor->SetRenderWindow(renderWindow);

	//设置交互方式
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> IStyle
		= vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	RenderWindowInteractor->SetInteractorStyle(IStyle);
}

void XRender::AddAllActors()
{
	for (int i = 0; i < Actors.size(); ++i)
		Renderer->AddActor(Actors[i]);
	for (int i = 0; i < Actor2Ds.size(); ++i)
		Renderer->AddActor2D(Actor2Ds[i]);
}

void XRender::Rendering()
{
	//坐标轴显示
	if (ShowAxes) {
		vtkNew<vtkAxesActor> axesActor;
		axesActor->SetPosition(0, 0, 0);
		axesActor->SetTotalLength(AxisLen, AxisLen, AxisLen);
		axesActor->SetShaftType(0);
		axesActor->SetAxisLabels(0);
		axesActor->SetCylinderRadius(0.01);
		Renderer->AddActor(axesActor);
	}

	AddAllActors();


	//#define MULTI_RENDERWINDOW
#ifdef MULTI_RENDERWINDOW
	std::thread my_threat(&vtkRenderWindowInteractor::Start, RenderWindowInteractor);
	my_threat.detach();		// 并发
	//my_threat.join();		// 串行
#else
	RenderWindowInteractor->Start();
#endif // MULTI_RENDERWINDOW

}


void XRender::OffScreenRendering(const std::string &filename)
{

	renderWindow->SetOffScreenRendering(1);
	renderWindow->Render();

	vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =
		vtkSmartPointer<vtkWindowToImageFilter>::New();
	windowToImageFilter->SetInput(renderWindow);
	//w2i->SetMagnification(1.0f);
	//w2i->ReadFrontBufferOn();
	//windowToImageFilter->ShouldRerenderOff(); //Set/get whether to re-render the input window. Initial value is true.
	windowToImageFilter->Update();

	vtkSmartPointer<vtkBMPWriter> writer = vtkSmartPointer<vtkBMPWriter>::New();
	writer->SetFileName(filename.c_str());
	writer->SetInputConnection(windowToImageFilter->GetOutputPort());
	writer->Write();
}


// -------------------------------------------------------------------
// Add Render Object
// -------------------------------------------------------------------

void XRender::AddMapper(vtkMapper *m)
{
	vtkNew<vtkActor> actor;
	actor->SetMapper(m);
	AddActor(actor);
}

void XRender::AddPolyData(vtkPolyData * pd)
{
	vtkNew<vtkPolyDataMapper> mapper;
	vtkNew<vtkActor> actor;
	mapper->SetInputData(pd);
	mapper->Update();
	actor->SetMapper(mapper);
	Actors.push_back(actor);
}

void XRender::AddPoints(vtkPoints *p, double sphereScale)
{
	vtkNew<vtkSphereSource> sphereSource;
	sphereSource->SetPhiResolution(16);
	sphereSource->SetThetaResolution(16);
	sphereSource->SetRadius(sphereScale);

	vtkNew<vtkPolyData> poly;
	poly->SetPoints(p);

	vtkNew<vtkGlyph3DMapper> gMapper;
	gMapper->SetInputData(poly);
	gMapper->SetSourceConnection(sphereSource->GetOutputPort());

	vtkNew<vtkActor> actor;
	actor->SetMapper(gMapper);
	AddActor(actor);
}
