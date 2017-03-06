#pragma once

class Vec2
{
public:
	float x, y;

public:
	Vec2() {}
	Vec2(const float* arr) { x = arr[0]; y = arr[1]; }
};

class Vec3
{
public:
	float x, y, z;

public:
	Vec3(){}
	Vec3(const float* arr) { x = arr[0]; y = arr[1]; z = arr[2]; }
};

class Vec4
{
public:
	float x, y, z, w;

public:
	Vec4() {}
	Vec4(const float* arr) { x = arr[0]; y = arr[1]; z = arr[2]; w = arr[3]; }
};

class Mat4
{
public:
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33 ,_34;
	float _41, _42, _43, _44;
public:
	Mat4() {}
	Mat4(float* array)
	{
		_11 = array[0]; _12 = array[1]; _13 = array[2]; _14 = array[3];
		_21 = array[4]; _22 = array[5]; _23 = array[6]; _24 = array[7];
		_31 = array[8]; _32 = array[9]; _33 = array[10]; _34 = array[11];
		_41 = array[12]; _42 = array[13]; _43 = array[14]; _44 = array[15];
	}
};