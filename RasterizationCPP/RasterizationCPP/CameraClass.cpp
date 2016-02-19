#include "CameraClass.h"

CameraClass::CameraClass(float aspect,float fov) {
	NearZ = 10.0f;
	FarZ = 1500.0f;
	FOV = fov;
	speed = 5.0f;

	screenAspect = aspect;

	Position.x = Position.y = Position.z = 0.0f;
	Position.z = -250.0f;
	Rotation.x = Rotation.y = Rotation.z = 0.0f;
}

//��ʱ�ò���
CameraClass::~CameraClass()
{
}

void CameraClass::Update(float newAspect, float fov) {
	screenAspect = newAspect;
	//FOV = fov;
	GetViewToHomoMatrix4();
}

void CameraClass::Update() {
	GetViewToHomoMatrix4();
}

Matrix4 CameraClass::GetWorldToViewMatrix4()
{
	Matrix4 WTV;
	WTV = Matrix4('A', Rotation)*Matrix4(Position);
	return WTV.Invert();
}

void CameraClass::GetViewToHomoMatrix4() {
	ViewToHomo.SetZero();
	float l, r, t, b;

	r = NearZ*tanf(DEGREE(FOV) / 2.0f);
	l = -r;
	t = r / screenAspect;
	b = -t;

	ViewToHomo.var[0][0] = (2 * NearZ) / (r - l);
	ViewToHomo.var[1][1] = (2 * NearZ) / (t - b);
	ViewToHomo.var[2][2] = (FarZ + NearZ) / (FarZ - NearZ);
	ViewToHomo.var[2][3] = 1.0f;
	ViewToHomo.var[3][2] = -(2.0f*NearZ*FarZ) / (FarZ - NearZ);
}