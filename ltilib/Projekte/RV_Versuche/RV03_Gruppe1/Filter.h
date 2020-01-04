#pragma once
#include "ltiImage.h"
namespace lti{
	class Filter
	{
	public:
		Filter(void);
		~Filter(void);
		void operator()(int argc,char *argv[]);
		static void idealLowpass(channel &creal, channel &cimag,channel &creal2, channel &cimag2,int fSize);
		static	void gaussLowpass(channel &creal, channel &cimag,channel &creal2, channel &cimag2,double sigma);
		static	void idealHighpass(channel &creal, channel &cimag,channel &creal2, channel &cimag2,int fSize);
		static	void gaussHighpass(channel &creal, channel &cimag,channel &creal2, channel &cimag2, double sigma);
		static	void Filter::highFreqEmph(channel &creal, channel &cimag,channel &creal2, channel &cimag2, double sigma, double a, double b);
		static	void laplaceGauss(channel &creal, channel &cimag,channel &creal2, channel &cimag2, double sigma);
		static	void sinnoise(channel &creal, channel &cimag,channel &creal2, channel &cimag2, int fSize1, int fSize2);
	};
}
