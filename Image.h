#include "stdafx.h"
#pragma once
#include "Array3D.h"


class Image
{
public:
	float bx;
	float by;
	float bz;

	float dx;
	float dy;
	float dz;

	size_t Nx;
	size_t Ny;
	size_t Nz;

	Array3D imData;

	Image(float bx_, float by_, float bz_, float dx_, float dy_, float dz_, size_t Nx_, size_t Ny_, size_t Nz_);
	Image(Image* src);
	void saveData(FILE* fp);
};
