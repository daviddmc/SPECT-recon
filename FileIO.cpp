#include "stdafx.h"
#include "FileIO.h"

#define _CRT_SECURE_NO_WARNINGS

long getLength(FILE *fp)
{
	long cur_pos;
	long len;
	cur_pos = ftell(fp);
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, cur_pos, SEEK_SET);
	return len;
}

void FileIO::readRaw(const char* filename, ProjData* projData)
{
	FILE* fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		printf("can not read file\n");
	}

	projData->readData(fp);

	fclose(fp);
}


void FileIO::savdImage(const char * filename, Image *img)
{
	FILE* fp = fopen(filename, "wb");
	if (fp == NULL)
	{
		printf("can not save file\n");
	}
	img->saveData(fp);
	//img->imData.saveData(fp);
	fclose(fp);
}