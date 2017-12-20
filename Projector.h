#include "stdafx.h"
#pragma once
#include <math.h>
#include "Image.h"
#include "ProjData.h"

class Projector
{
public:
	//Image * image;
	//ProjData * projData;
};

class BackProjector : public Projector
{
public:
	void backProject(Image * image, Image *  image2, float p1x, float p1y, float p2x, float p2y, int z, float projVal);
	//void backProject(Image* image, Image* image2, RawData rawEvent, float projVal);
};

class ForwardProjector : public Projector
{
public:
	void forwardProject(Image* image, float p1x, float p1y, float p2x, float p2y, int z, float& projVal);
};