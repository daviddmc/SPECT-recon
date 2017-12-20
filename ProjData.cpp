#include "stdafx.h"
#include "ProjData.h"


void ProjData::readData(FILE* fp)
{
	data.readData(fp);
}

size_t ProjData::size()
{
	return data.size();
}

size_t ProjData::size(int d)
{
	return data.size(d);
}

ProjData::ProjData(size_t numAngle, size_t numZ, size_t num_X):data(numAngle, numZ, num_X){}