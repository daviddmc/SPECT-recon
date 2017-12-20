#include "stdafx.h"
#include "Array3D.h"

Array3D::Array3D(size_t d1_, size_t d2_, size_t d3_) : 
	data(new float[d1_ * d2_ * d3_]), d1(d1_), d2(d2_), d3(d3_) {}

Array3D::~Array3D() { delete[] data; }

void Array3D::setAll(float val)
{
	size_t N = d1 * d2 * d3;
	for (size_t i = 0; i < N; i++)
	{
		data[i] = val;
	}
}

void Array3D::clear()
{
	setAll(0);
}

void Array3D::saveData(FILE* fp)
{
	fwrite(data, sizeof(float), d1*d2*d3, fp);
}

void Array3D::readData(FILE* fp)
{
	long numRead = fread(data, sizeof(float), d1*d2*d3, fp);
	
	if (numRead == d1*d2*d3)
	{
		printf("read %d event", numRead);
	}
	else
	{
		printf("read file error");
		//TODO
	}
}

float & Array3D::get(size_t i, size_t j, size_t k)
{
	return data[(i * d2 + j) * d3 + k];
}

size_t Array3D::size()
{
	return d1 * d2 * d3;
}

size_t Array3D::size(int d)
{
	if (d == 1)
		return d1;
	else if (d == 2)
		return d2;
	else if (d == 3)
		return d3;
	else
		return size();
}