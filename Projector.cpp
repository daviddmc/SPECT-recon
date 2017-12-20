#include "stdafx.h"
#include "Projector.h"


inline float phi(float alpha, float p1, float dp, float b, float d)
{
	return (p1 + alpha * dp - b) / d;
}

void forwardProject1y(Image* image, float p1x, float p1y, float p2x, float p2y, int k, float & projVal)
{
	int Nx = image->Nx + 1;
	int Ny = image->Ny + 1;
	int Nz = image->Nz + 1;

	int i = floorf((p1x - image->bx) / image->dx);

	if (i < 0 || k < 0 || i > Nx - 2 || k > Nz - 2)
	{
		//printf("forward1y\n");
		projVal = 0;
		return;
	}

	float d = 0;
	for (int j = 0; j <= Ny - 2; j++)
	{
		d += image->imData.get(i, j, k);
	}
	projVal = d * image->dy;
}

void forwardProject1x(Image* image, float p1x, float p1y, float p2x, float p2y, int k, float & projVal)
{
	int Nx = image->Nx + 1;
	int Ny = image->Ny + 1;
	int Nz = image->Nz + 1;

	int j = floorf((p1y - image->by) / image->dy);

	if (j < 0 || k < 0 || j > Ny - 2 || k > Nz - 2)
	{
		//printf("forward1x\n");
		projVal = 0;
		return;
	}

	float d = 0;
	for (int i = 0; i <= Nx - 2; i++)
	{
		d += image->imData.get(i,j,k);
	}
	projVal = d * image->dx;
}

void ForwardProjector::forwardProject(Image* image, float p1x, float p1y, float p2x, float p2y, int k, float& projVal)
{
	int Nx = image->Nx + 1;
	int Ny = image->Ny + 1;
	int Nz = image->Nz + 1;
	float dpx = p2x - p1x;
	float dpy = p2y - p1y;

	if (dpx == 0)
	{
		forwardProject1y(image, p1x, p1y, p2x, p2y, k, projVal);
		return;
	}

	if (dpy == 0)
	{
		forwardProject1x(image, p1x, p1y, p2x, p2y, k, projVal);
		return;
	}

	float alphaX0 = (image->bx - p1x) / dpx;
	float alphaXEnd = ((Nx - 1)*image->dx + image->bx - p1x) / dpx;
	float alphaY0 = (image->by - p1y) / dpy;
	float alphaYEnd = ((Ny - 1)*image->dy + image->by - p1y) / dpy;

	float alphaXMin = alphaX0 < alphaXEnd ? alphaX0 : alphaXEnd;
	float alphaXMax = alphaX0 > alphaXEnd ? alphaX0 : alphaXEnd;

	float alphaYMin = alphaY0 < alphaYEnd ? alphaY0 : alphaYEnd;
	float alphaYMax = alphaY0 > alphaYEnd ? alphaY0 : alphaYEnd;

	float alphaMin = fmaxf(alphaXMin, alphaYMin);
	float alphaMax = fminf(alphaXMax, alphaYMax);


	int iMin, iMax, jMin, jMax;
	if (p1x < p2x)
	{
		iMin = alphaMin == alphaXMin ? 1 : ceilf(phi(alphaMin, p1x, dpx, image->bx, image->dx));
		iMax = alphaMax == alphaXMax ? Nx - 1 : floorf(phi(alphaMax, p1x, dpx, image->bx, image->dx));
	}
	else
	{
		iMax = alphaMin == alphaXMin ? Nx - 2 : floorf(phi(alphaMin, p1x, dpx, image->bx, image->dx));
		iMin = alphaMax == alphaXMax ? 0 : ceilf(phi(alphaMax, p1x, dpx, image->bx, image->dx));
	}

	if (p1y < p2y)
	{
		jMin = alphaMin == alphaYMin ? 1 : ceilf(phi(alphaMin, p1y, dpy, image->by, image->dy));
		jMax = alphaMax == alphaYMax ? Ny - 1 : floorf(phi(alphaMax, p1y, dpy, image->by, image->dy));
	}
	else
	{
		jMax = alphaMin == alphaYMin ? Ny - 2 : floorf(phi(alphaMin, p1y, dpy, image->by, image->dy));
		jMin = alphaMax == alphaYMax ? 0 : ceilf(phi(alphaMax, p1y, dpy, image->by, image->dy));
	}

	float alphaX = p1x < p2x ? (iMin*image->dx + image->bx - p1x) / dpx : (iMax*image->dx + image->bx - p1x) / dpx;
	float alphaY = p1y < p2y ? (jMin*image->dy + image->by - p1y) / dpy : (jMax*image->dy + image->by - p1y) / dpy;

	float tmp = (fminf(alphaX, alphaY) + alphaMin) / 2.0;

	int i = floorf(phi(tmp, p1x, dpx, image->bx, image->dx));
	int j = floorf(phi(tmp, p1y, dpy, image->by, image->dy));

	if (i < 0 || j < 0 || k < 0 || i > Nx - 2 || j > Ny - 2 || k > Nz - 2)
	{
		//printf("forwardProject2z\n");
		projVal = 0;
		return;
	}

	float alphaC = alphaMin;

	float alphaXU = image->dx / fabsf(dpx);
	int iU = p1x < p2x ? 1 : -1;

	float alphaYU = image->dy / fabsf(dpy);
	int jU = p1y < p2y ? 1 : -1;

	float d = 0;
	while (true)
	{
		if (alphaX < alphaY)
		{
			if (alphaX != alphaC)
			{
				d += (alphaX - alphaC) * image->imData.get(i,j,k);
				alphaC = alphaX;
			}
			i += iU;
			alphaX += alphaXU;
			if (i < 0 || i > Nx - 2)
				break;
		}
		else
		{
			if (alphaY != alphaC)
			{
				d += (alphaY - alphaC) * image->imData.get(i,j,k);
				alphaC = alphaY;
			}
			j += jU;
			alphaY += alphaYU;
			if (j < 0 || j > Ny - 2)
				break;
		}
	}
	projVal = d;
}

void backProject1x(Image* image, Image* image2, float p1x, float p1y, float p2x, float p2y, int k, float projVal)
{
	int Nx = image->Nx + 1;
	int Ny = image->Ny + 1;
	int Nz = image->Nz + 1;

	int j = floorf((p1y - image->by) / image->dy);

	for (int i = 0; i <= Nx - 2; i++)
	{
		image->imData.get(i, j, k) += image->dx * projVal;
		image2->imData.get(i, j, k) += image->dx;
	}
}

void backProject1y(Image* image, Image* image2, float p1x, float p1y, float p2x, float p2y, int k, float projVal)
{
	int Nx = image->Nx + 1;
	int Ny = image->Ny + 1;
	int Nz = image->Nz + 1;

	int i = floorf((p1x - image->bx) / image->dx);

	for (int j = 0; j <= Ny - 2; j++)
	{
		image->imData.get(i, j, k) += image->dy * projVal;
		image2->imData.get(i, j, k) += image->dy;
	}
}

void BackProjector::backProject(Image * image, Image * image2, float p1x, float p1y, float p2x, float p2y, int k, float projVal)
{
	int Nx = image->Nx + 1;
	int Ny = image->Ny + 1;
	int Nz = image->Nz + 1;
	float dpx = p2x - p1x;
	float dpy = p2y - p1y;

	if (dpx == 0)
	{
		backProject1y(image, image2, p1x, p1y, p2x, p2y, k, projVal);
		return;
	}

	if (dpy == 0)
	{
		backProject1x(image, image2, p1x, p1y, p2x, p2y, k, projVal);
		return;
	}

	float alphaX0 = (image->bx - p1x) / dpx;
	float alphaXEnd = ((Nx - 1)*image->dx + image->bx - p1x) / dpx;
	float alphaY0 = (image->by - p1y) / dpy;
	float alphaYEnd = ((Ny - 1)*image->dy + image->by - p1y) / dpy;

	float alphaXMin = alphaX0 < alphaXEnd ? alphaX0 : alphaXEnd;
	float alphaXMax = alphaX0 > alphaXEnd ? alphaX0 : alphaXEnd;

	float alphaYMin = alphaY0 < alphaYEnd ? alphaY0 : alphaYEnd;
	float alphaYMax = alphaY0 > alphaYEnd ? alphaY0 : alphaYEnd;

	float alphaMin = fmaxf(alphaXMin, alphaYMin);
	float alphaMax = fminf(alphaXMax, alphaYMax);


	int iMin, iMax, jMin, jMax;
	if (p1x < p2x)
	{
		iMin = alphaMin == alphaXMin ? 1 : ceilf(phi(alphaMin, p1x, dpx, image->bx, image->dx));
		iMax = alphaMax == alphaXMax ? Nx - 1 : floorf(phi(alphaMax, p1x, dpx, image->bx, image->dx));
	}
	else
	{
		iMax = alphaMin == alphaXMin ? Nx - 2 : floorf(phi(alphaMin, p1x, dpx, image->bx, image->dx));
		iMin = alphaMax == alphaXMax ? 0 : ceilf(phi(alphaMax, p1x, dpx, image->bx, image->dx));
	}

	if (p1y < p2y)
	{
		jMin = alphaMin == alphaYMin ? 1 : ceilf(phi(alphaMin, p1y, dpy, image->by, image->dy));
		jMax = alphaMax == alphaYMax ? Ny - 1 : floorf(phi(alphaMax, p1y, dpy, image->by, image->dy));
	}
	else
	{
		jMax = alphaMin == alphaYMin ? Ny - 2 : floorf(phi(alphaMin, p1y, dpy, image->by, image->dy));
		jMin = alphaMax == alphaYMax ? 0 : ceilf(phi(alphaMax, p1y, dpy, image->by, image->dy));
	}

	float alphaX = p1x < p2x ? (iMin*image->dx + image->bx - p1x) / dpx : (iMax*image->dx + image->bx - p1x) / dpx;
	float alphaY = p1y < p2y ? (jMin*image->dy + image->by - p1y) / dpy : (jMax*image->dy + image->by - p1y) / dpy;

	float tmp = (fminf(alphaX, alphaY) + alphaMin) / 2.0;

	int i = floorf(phi(tmp, p1x, dpx, image->bx, image->dx));
	int j = floorf(phi(tmp, p1y, dpy, image->by, image->dy));
	/*
	if (i < 0 || j < 0 || k < 0 || i > Nx - 2 || j > Ny - 2 || k > Nz - 2)
	{
		return;
	}*/

	float alphaC = alphaMin;

	float alphaXU = image->dx / fabsf(dpx);
	int iU = p1x < p2x ? 1 : -1;

	float alphaYU = image->dy / fabsf(dpy);
	int jU = p1y < p2y ? 1 : -1;

	//float dConv = sqrtf((p1x - p2x)*(p1x - p2x) + (p1y - p2y)*(p1y - p2y));
	float d = 0;
	while (true)
	{
		if (alphaX < alphaY)
		{
			if (alphaX != alphaC)
			{
				tmp = (alphaX - alphaC);
				image->imData.get(i, j, k) += tmp * projVal;
				image2->imData.get(i,j,k) += tmp;
				alphaC = alphaX;
			}
			i += iU;
			alphaX += alphaXU;
			if (i < 0 || i > Nx - 2)
				break;
		}
		else
		{
			if (alphaY != alphaC)
			{
				tmp = (alphaY - alphaC);
				image->imData.get(i, j, k) += tmp * projVal;
				image2->imData.get(i, j, k) += tmp;
				alphaC = alphaY;
			}
			j += jU;
			alphaY += alphaYU;
			if (j < 0 || j > Ny - 2)
				break;
		}
	}
}
