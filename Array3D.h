#include "stdafx.h"
#pragma once

class Array3D
{
private:
	float * data;
	size_t d1, d2, d3;
public:
	Array3D(size_t d1_, size_t d2_, size_t d3_);
	~Array3D();
	Array3D& operator =(const Array3D& src) = delete;
	
	void clear();
	void setAll(float val);
	void saveData(FILE* fp);
	float & get(size_t i, size_t j, size_t k);
	void readData(FILE* fp);
	size_t size();
	size_t size(int d);
};
