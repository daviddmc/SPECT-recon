// SPECTrecon.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Array3D.h"
#include "Image.h"
#include "ProjData.h"
#include "FileIO.h"
#include "Projector.h"

int subsetSize[4] = { 16, 16, 14, 14 };
int subset1[16] = { 0,1,2,3,4,5,6,7,30,31,32,33,34,35,36,37 };
int subset2[16] = { 15,16,17,18,19,20,21,22,45,46,47,48,49,50,51,52 };
int subset3[14] = { 8,9,10,11,12,13,14,38,39,40,41,42,43,44 };
int subset4[14] = { 23,24,25,26,27,28,29,53,54,55,56,57,58,59 };
int *subset[4] = { subset1, subset2, subset3, subset4 };

const double PI = 3.14159265358979;
float cosList[60];
float sinList[60];

class SPECTrecon
{
public:
	BackProjector * backProjector;
	ForwardProjector* forwardProjector;
	ProjData* projData;
	Image* image;
	Image* imageTemp;
	Image* imageTemp2;
	FileIO* fileIO;

	int numIter;
	int numSubset;
	void recon(const char * outputFilename);
	void init(float bx, float by, float bz,
		float dx, float dy, float dz,
		size_t Nx, size_t Ny, size_t Nz,
		const char * inputFilename);

	SPECTrecon(int niter, int nsub)
	{
		backProjector = new BackProjector();
		forwardProjector = new ForwardProjector();
		projData = new ProjData(60, 128, 128);
		image = NULL;
		imageTemp = NULL;
		imageTemp2 = NULL;
		fileIO = new FileIO();
		numIter = niter;
		numSubset = nsub;
	}
};

void SPECTrecon::recon(const char * outputFilename)
{
	numSubset = 4;
	//int subsetSize = projData->size() / numSubset;
	//printf("number of subset: %d, size of each subset: %d\n", numSubset, subsetSize);

	size_t numAngle = projData->size(1);
	size_t numZ = projData->size(2);
	size_t numX = projData->size(3);

	printf("recon start\n");
	for (int iter = 0; iter < numIter; iter++)
	{
		printf("iter %d start\n", iter);
		for (int sub = 0; sub < numSubset; sub++)
		{
			printf("subiter %d start\n", sub);
			//int count = 0;
			
			for (int a = 0; a < subsetSize[sub]; a++)
			{
				int i = subset[sub][a];
				printf("new angle");
				for (int j = 0; j < numZ; j++)
				{
					for (int k = 0; k < numX; k++)
					{
						float s = 4.0625 * (k - 63.5);
						float p1x = s * sinList[i];
						float p1y = s * cosList[i];
						float p2x = p1x + cosList[i];
						float p2y = p1y - sinList[i];
						float tmp;
						forwardProjector->forwardProject(image, p1x, p1y, p2x, p2y, j, tmp);
						if (tmp > 0)
						{
							tmp = projData->data.get(i, j, k) / tmp;
							backProjector->backProject(imageTemp, imageTemp2, p1x, p1y, p2x, p2y, j, tmp);
						}

					}
				}
			}
			
			for (int x = 0; x < image->Nx; x++)
			{
				for (int y = 0; y < image->Ny; y++)
				{
					for (int z = 0; z < image->Nz; z++)
					{
						if (imageTemp2->imData.get(x, y, z) > 0)
							image->imData.get(x, y, z) *= imageTemp->imData.get(x,y,z) / imageTemp2->imData.get(x, y, z);
						imageTemp->imData.get(x, y, z) = 0;
						imageTemp2->imData.get(x, y, z) = 0;
					}
				}
			}
			printf("subiter %d end\n", sub);
		}
		printf("iter %d end\n", iter);
	}
	fileIO->savdImage(outputFilename, image);
	printf("recon end\n");

}

void SPECTrecon::init(float bx, float by, float bz,
	float dx, float dy, float dz,
	size_t Nx, size_t Ny, size_t Nz,
	const char * inputFilename)
{
	image = new Image(bx, by, bz, dx, dy, dz, Nx, Ny, Nz);
	//image->imData.setAll(1);
	int rx = image->Nx / 2;
	int ry = image->Ny / 2;
	for (int i = 0; i < image->Nx; i++)
	{
		for (int j = 0; j < image->Ny; j++)
		{
			for (int k = 0; k < image->Nz; k++) {
				if ((i - rx)*(i - rx) + (j - ry)*(j - ry) < rx*ry)
				{
					image->imData.get(i,j,k) = 0.5;
				}
				else
				{
					image->imData.get(i, j, k) = 0;
				}
			}
		}
	}

	imageTemp = new Image(image);
	imageTemp->imData.clear();
	imageTemp2 = new Image(image);
	imageTemp2->imData.clear();
	fileIO->readRaw(inputFilename, projData);
	//projData->rawData = fileIO->readRaw(inputFilename, projData->N);
	//projData->data = new float[projData->N];
}

int main()
{
	for (int i = 0; i < 60; i++)
	{
		cosList[i] = cos(i * 6.0 * PI / 180.0);
		sinList[i] = sin(i * 6.0 * PI / 180.0);
	}

	size_t Nx = 128;
	size_t Ny = 128;
	size_t Nz = 128;

	float dx = 4.0625;
	float dy = 4.0625;
	float dz = 3.125;

	float bx = -260.9;
	float by = -260.0;
	float bz = -199.68;

	SPECTrecon reconObject = SPECTrecon(4, 4);
	reconObject.init(bx, by, bz, dx, dy, dz, Nx, Ny, Nz, "../data/whole_bone.raw");
	reconObject.recon("../data/whole_bone_output");

	return 0;
}

