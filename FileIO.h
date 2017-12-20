#pragma once
#include "stdafx.h"
#include "ProjData.h"
#include "Image.h"
#include <stdio.h>

class FileIO
{
public:
	void readRaw(const char* filename, ProjData* projData);
	void savdImage(const char * filename, Image *img);
};