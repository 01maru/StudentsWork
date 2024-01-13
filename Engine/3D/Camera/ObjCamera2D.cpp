#include "ObjCamera2D.h"
#include "Window.h"

MNE::ObjCamera2D::ObjCamera2D()
{
	eye_ = { 0.0f,0.0f,-100.0f };
	target_ = { 0.0f,0.0f,0.0f };
	up_ = { 0.0f,1.0f,0.0f };

	matProj_ = MyMath::OrthoLH(MNE::Window::sWIN_WIDTH, MNE::Window::sWIN_HEIGHT, 0.1f, 1000.0f);

	MatUpdate();

	CalcDirectionVec();
}
