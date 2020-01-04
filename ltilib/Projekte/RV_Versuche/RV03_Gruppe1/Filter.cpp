#include "ltiObject.h"
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <stdio.h>
#include "ltiImage.h"
#include "ltiBMPFunctor.h"
#include "ltiViewer.h"
#include "ltiSplitImg.h"
#include "gtk.h"
#include "ltiGtkServer.h"
#include "ltiRealFFT.h"
#include "ltiRealInvFFT.h"
#include "ltiHistogramEqualization.h"
#include "RV03.h"
#include "Filter.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace lti{

Filter::Filter(void)
{
}

Filter::~Filter(void)
{
}

void Filter::idealLowpass(channel &creal, channel &cimag,channel &creal2, channel &cimag2,int fSize){
	int PicSizeX = creal.columns() , PicSizeY = creal.rows();
	int PicMidX = PicSizeX/2 , PicMidY = PicSizeY/2;
	float euklidX = 0, euklidY = 0;
	float eukDist = 0, filterKoeff = 0;

	for(int y = 0; y < PicSizeY; y++)
	{
		for(int x = 0; x < PicSizeX; x++)
		{
			euklidX = x-PicMidX;
			euklidY = y-PicMidY;
			eukDist = sqrt(pow(euklidX,2)+pow(euklidY,2));
			filterKoeff = 0.0;
			if (eukDist <= fSize)
				filterKoeff = 1.0;
			creal2[y][x] = filterKoeff * creal[y][x];
			cimag2[y][x] = filterKoeff * cimag[y][x];
		}
	}
}

void Filter::gaussLowpass(channel &creal, channel &cimag,channel &creal2, channel &cimag2, double sigma){
	int PicSizeX = creal.columns() , PicSizeY = creal.rows();
	int PicMidX = PicSizeX/2 , PicMidY = PicSizeY/2;
	float euklidX = 0, euklidY = 0;
	float normEukDist = 0, filterKoeff = 0;

	for(int y = 0; y < PicSizeY; y++)
	{
		for(int x = 0; x < PicSizeX; x++)
		{
			euklidX = x-PicMidX;
			euklidY = y-PicMidY;
			normEukDist = sqrt(pow(euklidX/PicSizeX,2)+pow(euklidY/PicSizeY,2));
			filterKoeff = pow(std::exp(1.0),-2*pow(M_PI,2)*pow(sigma,2)*pow(normEukDist,2));
			creal2[y][x] = filterKoeff * creal[y][x];
			cimag2[y][x] = filterKoeff * cimag[y][x];
		}
	}
}
void Filter::idealHighpass(channel &creal, channel &cimag,channel &creal2, channel &cimag2,int fSize){
	int PicSizeX = creal.columns() , PicSizeY = creal.rows();
	int PicMidX = PicSizeX/2 , PicMidY = PicSizeY/2;
	float euklidX = 0, euklidY = 0;
	float eukDist = 0, filterKoeff = 0;

	for(int y = 0; y < PicSizeY; y++)
	{
		for(int x = 0; x < PicSizeX; x++)
		{
			euklidX = x-PicMidX;
			euklidY = y-PicMidY;
			eukDist = sqrt(pow(euklidX,2)+pow(euklidY,2));
			filterKoeff = 1.0;
			if (eukDist <= fSize)
				filterKoeff = 0.0;
			creal2[y][x] = filterKoeff * creal[y][x];
			cimag2[y][x] = filterKoeff * cimag[y][x];
		}
	}
}
void Filter::gaussHighpass(channel &creal, channel &cimag,channel &creal2, channel &cimag2, double sigma){
	int PicSizeX = creal.columns() , PicSizeY = creal.rows();
	int PicMidX = PicSizeX/2 , PicMidY = PicSizeY/2;
	float euklidX = 0, euklidY = 0;
	float normEukDist = 0, filterKoeff = 0;

	for(int y = 0; y < PicSizeY; y++)
	{
		for(int x = 0; x < PicSizeX; x++)
		{
			euklidX = x-PicMidX;
			euklidY = y-PicMidY;
			normEukDist = sqrt(pow(euklidX/PicSizeX,2)+pow(euklidY/PicSizeY,2));
			filterKoeff = 1.0 - pow(std::exp(1.0),-2*pow(M_PI,2)*pow(sigma,2)*pow(normEukDist,2));
			creal2[y][x] = filterKoeff * creal[y][x];
			cimag2[y][x] = filterKoeff * cimag[y][x];
		}
	}
}
void Filter::highFreqEmph(channel &creal, channel &cimag,channel &creal2, channel &cimag2, double sigma, double a, double b){
	int PicSizeX = creal.columns() , PicSizeY = creal.rows();
	int PicMidX = PicSizeX/2 , PicMidY = PicSizeY/2;
	float euklidX = 0, euklidY = 0;
	float normEukDist = 0, filterKoeff = 0;

	for(int y = 0; y < PicSizeY; y++)
	{
		for(int x = 0; x < PicSizeX; x++)
		{
			euklidX = x-PicMidX;
			euklidY = y-PicMidY;
			normEukDist = sqrt(pow(euklidX/PicSizeX,2)+pow(euklidY/PicSizeY,2));
			filterKoeff = a + b*(1.0 - pow(std::exp(1.0),-2*pow(M_PI,2)*pow(sigma,2)*pow(normEukDist,2)));
			creal2[y][x] = filterKoeff * creal[y][x];
			cimag2[y][x] = filterKoeff * cimag[y][x];
		}
	}
}
void Filter::laplaceGauss(channel &creal, channel &cimag,channel &creal2, channel &cimag2, double sigma){
	int PicSizeX = creal.columns() , PicSizeY = creal.rows();
	int PicMidX = PicSizeX/2 , PicMidY = PicSizeY/2;
	float euklidX = 0, euklidY = 0;
	float normEukDist = 0, filterKoeff = 0;

	for(int y = 0; y < PicSizeY; y++)
	{
		for(int x = 0; x < PicSizeX; x++)
		{
			euklidX = x-PicMidX;
			euklidY = y-PicMidY;
			normEukDist = sqrt(pow(euklidX/PicSizeX,2)+pow(euklidY/PicSizeY,2));
			filterKoeff = -4*pow(M_PI,2)*pow(normEukDist,2)*pow(std::exp(1.0),-2*pow(M_PI,2)*pow(sigma,2)*pow(normEukDist,2));
			creal2[y][x] = filterKoeff * creal[y][x];
			cimag2[y][x] = filterKoeff * cimag[y][x];
		}
	}
}
void Filter::sinnoise(channel &creal, channel &cimag,channel &creal2, channel &cimag2, int fSize1, int fSize2){
	
	int PicSizeX = creal.columns() , PicSizeY = creal.rows();
	int PicMidX = PicSizeX/2 , PicMidY = PicSizeY/2;
	float euklidX = 0, euklidY = 0;
	float eukDist = 0, filterKoeff = 0;

	int minDist = fSize1/2;
	int maxDist = fSize2/2;

	for(int y = 0; y < PicSizeY; y++)
	{
		for(int x = 0; x < PicSizeX; x++)
		{
			filterKoeff = 1.0;
			if (y > PicMidY+minDist && y < PicMidY+maxDist)
			{
				double pos = y-PicMidY;
				double maxPos = (double)maxDist - (double)minDist;
				filterKoeff = 1.0-sin((M_PI/180*(pos/maxPos*360.0)/2));
			}
			else if (y < PicMidY-minDist && y > PicMidY-maxDist)
			{
				double pos = -y+PicMidY;
				double maxPos = (double)maxDist - (double)minDist;
				filterKoeff = 1.0-sin((M_PI/180*(pos/maxPos*360.0)/2));
			}
			else if (x > PicMidX+minDist && x < PicMidX+maxDist)
			{
				double pos = x-PicMidX;
				double maxPos = (double)maxDist - (double)minDist;
				filterKoeff = 1.0-sin((M_PI/180*(pos/maxPos*360.0)/2));
			}
			else if (x < PicMidX-minDist && x > PicMidX-maxDist)
			{
				double pos = -x+PicMidX;
				double maxPos = (double)maxDist - (double)minDist;
				filterKoeff = 1.0-sin((M_PI/180*(pos/maxPos*360.0)/2));
			}
			creal2[y][x] = filterKoeff * creal[y][x];
			cimag2[y][x] = filterKoeff * cimag[y][x];
		}
	}
}

};
