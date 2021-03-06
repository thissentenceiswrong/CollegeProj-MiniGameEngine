#include "Utility.h"

//为了用 static
#include "WindowFrameClass.h"

void ClippingAndBackCull(vector<Fragment> &rl) {
	for (auto &item : rl) {
		if ((item.trans_vList[0].z > 0) && (item.trans_vList[0].x <  item.trans_vList[0].w && item.trans_vList[0].x > -item.trans_vList[0].w) && (item.trans_vList[0].y <  item.trans_vList[0].w && item.trans_vList[0].y > -item.trans_vList[0].w) &&
			(item.trans_vList[1].z > 0) && (item.trans_vList[1].x <  item.trans_vList[1].w && item.trans_vList[1].x > -item.trans_vList[1].w) && (item.trans_vList[1].y < item.trans_vList[1].w && item.trans_vList[1].y > -item.trans_vList[1].w) &&
			(item.trans_vList[2].z > 0) && (item.trans_vList[2].x < item.trans_vList[2].w && item.trans_vList[2].x > -item.trans_vList[2].w) && (item.trans_vList[2].y < item.trans_vList[2].w && item.trans_vList[2].y > -item.trans_vList[2].w)) {
			;
		}
		else{
			item.state = item.state | FRAGMENT_DELETED;
			continue;
		}

		Vector3 a(item.trans_vList[0].x - item.trans_vList[1].x, item.trans_vList[0].y - item.trans_vList[1].y, item.trans_vList[0].z - item.trans_vList[1].z);
		Vector3 b(item.trans_vList[2].x - item.trans_vList[1].x, item.trans_vList[2].y - item.trans_vList[1].y, item.trans_vList[2].z - item.trans_vList[1].z);
		item.n = a.CrossProduct(b);
		Vector3 view(item.trans_vList[0].x, item.trans_vList[0].y, item.trans_vList[0].z);
		if (view.DotProduct(item.n) < 0) {
			item.state = item.state | FRAGMENT_DELETED;
		}
	}
}

void HomoToScreenCoord(Vector4& vertex) {
	const RECT &screen = WindowFrame::rect_client;
	vertex.x = (vertex.x + 1.0f) * screen.right / 2.0f;
	vertex.y = ((-vertex.y + 1.0f) * screen.bottom / 2.0f);
}

/*************************
Vector3
*************************/

inline void Vector3::VectorUnify() {
	float length = x * x + y * y + z * z;
	length = sqrtf(length);
	x /= length;
	y /= length;
	z /= length;
}

inline Vector3 Vector3::CrossProduct(const Vector3& b) {
	Vector3 output;
	output.x = y*b.z - z*b.y;
	output.y = z*b.x - x*b.z;
	output.z = x*b.y - y*b.x;
	return output;
}

inline float Vector3::DotProduct(const Vector3& b) {
	return x*b.x + y*b.y + z*b.z;
}

Matrix4 Vector3::GetTransitonMatrix() {
	Matrix4 matrix_T;

	matrix_T.var[0][0] = 1.0f;
	matrix_T.var[1][1] = 1.0f;
	matrix_T.var[2][2] = 1.0f;
	matrix_T.var[3][3] = 1.0f;

	matrix_T.var[3][0] = x;
	matrix_T.var[3][1] = y;
	matrix_T.var[3][2] = z;

	return matrix_T;
}

Vector3 Vector3::operator * (const Matrix3 &b) {
	return Vector3(
		x*b.var[0][0] + y*b.var[1][0] + z*b.var[2][0],
		x*b.var[0][1] + y*b.var[1][1] + z*b.var[2][1],
		x*b.var[0][2] + y*b.var[1][2] + z*b.var[2][2]);
}

Vector4 Vector3::operator * (const Matrix4 &b) {
	return Vector4(*this,true) * b;
}

Vector3 Vector3::operator+(const Vector3 &b) {
	return Vector3(x + b.x, y + b.y, z + b.z);
}

Vector3 Vector3::operator-(const Vector3 &b) {
	return Vector3(x - b.x, y - b.y, z - b.z);
}

wstringstream &operator << (wstringstream& ws, const Vector3& v) {
	ws << "[ " << v.x << ", " << v.y << ", " << v.z << " ] ";
	return ws;
}

/*************************
Vector4
*************************/

Vector4 Vector4::operator*(const Matrix4 & b) {
	Vector4 tmp;
	tmp.x = x*b.var[0][0] + y*b.var[1][0] + z*b.var[2][0] + w * b.var[3][0];
	tmp.y = x*b.var[0][1] + y*b.var[1][1] + z*b.var[2][1] + w * b.var[3][1];
	tmp.z = x*b.var[0][2] + y*b.var[1][2] + z*b.var[2][2] + w * b.var[3][2];
	tmp.w = x*b.var[0][3] + y*b.var[1][3] + z*b.var[2][3] + w * b.var[3][3];
	return tmp;
}

void Vector4::VectorUnify() {
	float length = x * x + y * y + z * z;
	length = sqrtf(length);
	x /= length;
	y /= length;
	z /= length;
}

wstringstream& operator << (wstringstream& ws, const Vector4& v) {
	ws << "[ " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " ]";
	return ws;
}


/************************
MARTIX
3 x 3
*************************/
Matrix3::Matrix3() {
	memset(var, 0, 3 * 3 * sizeof(float));
}

Matrix3::Matrix3(const Matrix3 &old) {
	memcpy(var, old.var, 3 * 3 * sizeof(float));
}

void Matrix3::SetZero() {
	memset(var, 0, 3 * 3 * sizeof(float));
}

Matrix3 Matrix3::operator*(const Matrix3 &b) {
	Matrix3 martix;
	for (int lop = 0; lop < 3; lop++)
	{
		for (int lop2 = 0; lop2 < 3; lop2++)
		{
			martix.var[lop][lop2] =
				(var[lop][0] * b.var[0][lop2]) +
				(var[lop][1] * b.var[1][lop2]) +
				(var[lop][2] * b.var[2][lop2]);
		}
	}
	return martix;
}

Matrix3 Matrix3::operator*(const float &multi) {
	Matrix3 tmp = *this;
	tmp.var[0][0] *= multi;
	tmp.var[1][1] *= multi;
	tmp.var[2][2] *= multi;

	return tmp;
}


/****************************
Martix 
4 x 4
**********************/

Matrix4::Matrix4() {
	SetZero();
}

//平移矩阵
Matrix4::Matrix4(const Vector3& transition) {
	SetZero();

	var[0][0] = 1.0f;
	var[1][1] = 1.0f;
	var[2][2] = 1.0f;
	var[3][3] = 1.0f;

	var[3][0] = transition.x;
	var[3][1] = transition.y;
	var[3][2] = transition.z;
}

Matrix4::Matrix4(float x, float y, float z) {
	SetZero();

	var[0][0] = 1.0f;
	var[1][1] = 1.0f;
	var[2][2] = 1.0f;
	var[3][3] = 1.0f;

	var[3][0] = x;
	var[3][1] = y;
	var[3][2] = z;
}

//单轴旋转矩阵
Matrix4::Matrix4(char axis, float degree) {
	SetZero();

	var[0][0] = 1.0f;
	var[1][1] = 1.0f;
	var[2][2] = 1.0f;
	var[3][3] = 1.0f;

	//如果没进行旋转
	if (-0.001f <= degree && degree <= 0.001f) {
		return;
	}

	degree = DEGREE(degree);
	float c = cosf(degree);
	float s = sinf(degree);

	switch (axis)
	{
	case 'x':
	case 'X':
		var[1][1] = c;
		var[2][2] = c;
		var[1][2] = s;
		var[2][1] = -s;
		break;
	case 'y':
	case 'Y':
		var[0][0] = c;
		var[2][2] = c;
		var[2][0] = s;
		var[0][2] = -s;
		break;
	case 'z':
	case 'Z':
		var[0][0] = c;
		var[1][1] = c;
		var[0][1] = s;
		var[1][0] = -s;
		break;
	//Should never reach here
	default:
		break;
	}
}

Matrix4::Matrix4(Vector3 scale, Vector3 Rotation, Vector3 Position) {
	*this = Matrix4().createScaleMatrix4(scale) * Matrix4().createRotationMatrix4(Rotation) * Matrix4().createTransitionMatrix4(Position);
}

Matrix4::Matrix4(Matrix4 &a, const Matrix4 &b) {
	*this = a*b;
}

//复制构造函数
Matrix4::Matrix4(const Matrix4 &old) {
	memcpy(var, old.var, 4 * 4 * sizeof(float));
}

//将当前矩阵设为平移矩阵
Matrix4& Matrix4::createTransitionMatrix4(const Vector3 &transition) {
	SetZero();

	var[0][0] = 1.0f;
	var[1][1] = 1.0f;
	var[2][2] = 1.0f;
	var[3][3] = 1.0f;

	var[3][0] = transition.x;
	var[3][1] = transition.y;
	var[3][2] = transition.z;

	return *this;
}

Matrix4& Matrix4::createTransitionMatrix4(float x, float y, float z) {
	SetZero();

	var[0][0] = 1.0f;
	var[1][1] = 1.0f;
	var[2][2] = 1.0f;
	var[3][3] = 1.0f;

	var[3][0] = x;
	var[3][1] = y;
	var[3][2] = z;

	return *this;
}

//给Vector3创造旋转矩阵
Matrix4& Matrix4::createRotationMatrix4(const Vector3& b) {
	*this = Matrix4('x', b.x) * Matrix4('y', b.y) * Matrix4('z', b.z);
	return *this;
}

//给出3个轴的旋转角，创建旋转矩阵
Matrix4& Matrix4::createRotationMatrix4(float x, float y, float z) {
	*this = Matrix4('x', x)*Matrix4('y', y)*Matrix4('z', z);
	return *this;
}

//缩放矩阵
Matrix4& Matrix4::createScaleMatrix4(const Vector3 &factor) {
	SetZero();

	var[0][0] = factor.x;
	var[1][1] = factor.y;
	var[2][2] = factor.z;

	var[3][3] = 1;
	return *this;
}
Matrix4& Matrix4::createScaleMatrix4(float x, float y, float z) {
	SetZero(); 
	
	var[0][0] = x;
	var[1][1] = y;
	var[2][2] = z;

	var[3][3] = 1;
	return *this;
}

//在当前矩阵基础上进行操作
//平移
Matrix4& Matrix4::changePosition(const Vector3& t) {
	var[3][0] += t.x;
	var[3][1] += t.y;
	var[3][2] += t.z;
	return *this;
}
Matrix4& Matrix4::changePosition(float x, float y, float z) {
	var[3][0] += x;
	var[3][1] += y;
	var[3][2] += z;
	return *this;
}

//旋转
Matrix4& Matrix4::changeRotationSingleAxis(char axis, float degree) {
	*this = *this * Matrix4(axis, degree);
	return *this;
}
Matrix4& Matrix4::changeRotationMultiAxes(const Vector3& t) {
	this->changeRotationSingleAxis('x', t.x).changeRotationSingleAxis('y', t.y).changeRotationSingleAxis('z', t.z);
	return *this;
}
Matrix4& Matrix4::changeRotationMultiAxes(float x, float y, float z) {
	this->changeRotationSingleAxis('x', x).changeRotationSingleAxis('y', y).changeRotationSingleAxis('z', z);
	return *this;
}

//缩放
Matrix4& Matrix4::changeScale(const Vector3& t) {
	*this = *this * Matrix4().createScaleMatrix4(t);
	return *this;
}
Matrix4& Matrix4::changeScale(float x, float y, float z) {
	*this = *this * Matrix4().createScaleMatrix4(x, y, z);
	return *this;
}

//求余子式
float Matrix4::Determinant(const Matrix3 &input) {
	return
		(input.var[0][0] * input.var[1][1] * input.var[2][2]) +
		(input.var[0][1] * input.var[1][2] * input.var[2][0]) +
		(input.var[0][2] * input.var[1][0] * input.var[2][1]) -
		(input.var[0][2] * input.var[1][1] * input.var[2][0]) -
		(input.var[0][1] * input.var[1][0] * input.var[2][2]) -
		(input.var[0][0] * input.var[1][2] * input.var[2][1]);
}

//求逆矩阵
Matrix4& Matrix4::Invert() {
	Matrix4 output;
	//用于储存每次的余子式
	Matrix3 tmp;

	//////////////////////////
	// 为什么又错了？？？ //
	//////////////////////////
	/*
	float firstPart;
	//先算分母，再求倒
	firstPart =
		(var[0][0] * var[1][1] * var[2][2] * var[3][3]) +
		(var[0][1] * var[1][2] * var[2][3] * var[3][0]) +
		(var[0][2] * var[1][3] * var[2][0] * var[3][1]) +
		(var[0][3] * var[1][0] * var[2][1] * var[3][2]) -
		(var[0][3] * var[1][2] * var[2][1] * var[3][0]) -
		(var[0][2] * var[1][1] * var[2][0] * var[3][3]) -
		(var[0][1] * var[1][0] * var[2][3] * var[3][2]) -
		(var[0][0] * var[1][3] * var[2][2] * var[3][1]);

	//求倒
	firstPart = 1.0f / firstPart;
	*/

	//最里层的循环
	int x, y;
	//用于标识行列式的x y
	int x_tmp, y_tmp;

	for (int lop = 0; lop < 4; lop++) {
		for (int lop2 = 0; lop2 < 4; lop2++) {
			tmp.SetZero();
			x_tmp = 0;
			y_tmp = 0;
			//为output矩阵的元素求值
			//一行行一列列的进行计算
			for (x = 0; x < 4; x++) {
				y_tmp = 0;
				//跳过当前元素
				if (x == lop) {
					continue;
				}
				for (y = 0; y < 4; y++) {
					//跳过当前元素
					if (y == lop2) {
						continue;
					}
					tmp.var[x_tmp][y_tmp++] = var[x][y];
				}
				x_tmp++;
			}

			output.var[lop2][lop] = Determinant(tmp);

			//带-1
			if ((lop + 1 + lop2 + 1) % 2) {
				output.var[lop2][lop] = (-1.0f)*output.var[lop2][lop];
			}
		}
	}
	//output = output * firstPart;
	*this = output;

	return *this;
}

//矩阵相乘
Matrix4 Matrix4::operator*(const Matrix4 &b) {
	Matrix4 martix;
	for (int lop = 0; lop < 4; lop++) {
		for (int lop2 = 0; lop2 < 4; lop2++) {
			martix.var[lop][lop2] =
				(var[lop][0] * b.var[0][lop2]) +
				(var[lop][1] * b.var[1][lop2]) +
				(var[lop][2] * b.var[2][lop2]) +
				(var[lop][3] * b.var[3][lop2]);
		}
	}
	return martix;
}

//矩阵乘以数字
Matrix4 Matrix4::operator*(const float &multi) {
	Matrix4 tmp = *this;
	tmp.var[0][0] *= multi;
	tmp.var[1][1] *= multi;
	tmp.var[2][2] *= multi;
	tmp.var[3][3] *= multi;
	return tmp;
}