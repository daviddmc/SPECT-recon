#include "stdafx.h"
#include "Image.h"


Image::Image(float bx_, float by_, float bz_, 
	float dx_, float dy_, float dz_,
	size_t Nx_, size_t Ny_, size_t Nz_) :
	bx(bx_), by(by_), bz(bz_), 
	dx(dx_), dy(dy_), dz(dz_), 
	Nx(Nx_), Ny(Ny_), Nz(Nz_), 
	imData(Nx_, Ny_, Nz_) {}

Image::Image(Image* src) :
	bx(src->bx), by(src->by), bz(src->bz), 
	dx(src->dx), dy(src->dy), dz(src->dz),
	Nx(src->Nx), Ny(src->Ny), Nz(src->Nz), 
	imData(src->Nx, src->Ny, src->Nz) {}

void Image::saveData(FILE* fp)
{
	imData.saveData(fp);
}