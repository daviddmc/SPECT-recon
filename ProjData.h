#include "stdafx.h"
#pragma once
#include "Array3D.h"


class ProjData
{
public:
	//float dr;
	//float dt;
	//float dz;
	Array3D data;
	void readData(FILE* fp);
	size_t size();
	size_t size(int d);
	ProjData(size_t numAngle, size_t numZ, size_t num_X);
};

