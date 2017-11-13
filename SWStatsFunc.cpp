//////////////////////////////////////////////////////////////
//
// Your #includes here; make sure you are allowed them ...
//

#include <stdlib.h>
#include <limits>
#include <float.h>
#include <math.h>

//////////////////////////////////////////////////////////////
//
// #includes and function declarations; do not remove
//

#ifndef MARMOSET_TESTING

#include <iostream>

using namespace std;

int SWStats(const float dataset[], const int size,
	    const int currentSample, const int windowSize,
	    float& min, float& avg, float& max,
	    float& smplSD, float& mdn);

int SWMinimum(const float dataset[], const int size,
	      const int currentSample, const int windowSize,
	      float& minimum);
int SWAverage(const float dataset[], const int size,
	      const int currentSample, const int windowSize,
	      float& average);
int SWMaximum(const float dataset[], const int size,
	      const int currentSample, const int windowSize,
	      float& maximum);
int SWSmplStdDev(const float dataset[], const int size,
		 const int currentSample, const int windowSize,
		 float& smplStdDev);
int SWMedian(const float dataset[], const int size,
	     const int currentSample, const int windowSize,
	     float& median);
#endif

//////////////////////////////////////////////////////////////
//
// Your code here ...
//  
void* Change(float value[],const float dataset[],const int windowSize,const int currentSample)
{
    int count = 0;
    while (count < windowSize) {
		if((currentSample-count)<0)
			value[count] = dataset[0];
		else
        value[count] = dataset[currentSample-count];
        count++;
		for(int i=0;i<windowSize;i++){
			cout<<value[i]<<" ";
			cout<<endl;
		}
    }
}

int SWStats(const float dataset[], const int size,
	    const int currentSample, const int windowSize,
	    float& min, float& avg, float& max,
	    float& smplSD, float& mdn) {
	if(size<1)
		return -1;
	if(windowSize<=0)
		return -1;
	if(windowSize>=size)
		return 1;
	if(windowSize==1)
		return 1;
	SWMinimum(dataset,size,currentSample,windowSize,min);
	SWAverage(dataset,size,currentSample,windowSize,avg);
	SWMaximum(dataset,size,currentSample,windowSize,max);
	SWSmplStdDev(dataset,size,currentSample,windowSize,smplSD);
	SWMedian(dataset,size,currentSample,windowSize,mdn);
	return 0;
}

int SWMinimum(const float dataset[], const int size,
	      const int currentSample, const int windowSize,
	      float& minimum) {
	float value [windowSize];
	int a=0;
	Change(value,dataset,windowSize,currentSample);
    minimum = value[0];
    while (a < windowSize) {
        if ((value[a]) < minimum)
            minimum = value[a];
        a++;
    }
    return 0;      
}

int SWAverage(const float dataset[], const int size,
	      const int currentSample, const int windowSize,
	      float& average) {
	float value [windowSize];
	int a=0;
	Change(value,dataset,windowSize,currentSample);
    float sum=0;
    while (a < windowSize) {
        sum=sum+value[a];
        a++;
    }
	average=sum/windowSize;
    return 0;     
}

int SWMaximum(const float dataset[], const int size,
	      const int currentSample, const int windowSize,
	      float& maximum) {
	float value [windowSize];
	int a=0;
	Change(value,dataset,windowSize,currentSample);
	maximum = value[0];
    while (a < windowSize) {
        if ((value[a]) > maximum)
            maximum = value[a];
        a++;
    }
    return 0;  
}

int SWSmplStdDev(const float dataset[], const int size,
		 const int currentSample, const int windowSize,
		 float& smplStdDev) {
	if(windowSize<2)
		return std::numeric_limits<float>::quiet_NaN();
	float value [windowSize];
	float average;
	SWAverage(dataset,size,currentSample,windowSize,average);
	Change(value,dataset,windowSize,currentSample);
	int i=0;
	float sum=0;
	while(i<windowSize){
		sum=sum+((value[i]-average)*(value[i]-average));
		i++;
	}
	smplStdDev=sqrt(sum / (windowSize-1));
	return 0;

}

int SWMedian(const float dataset[], const int size,
	     const int currentSample, const int windowSize,
	     float& median) {
	float A[windowSize];
	float value[windowSize];
	Change(value,dataset,windowSize,currentSample);
    for(int i = 0; i < windowSize; i++) {
		A[i] = value[i];
    }
	for(int n = windowSize; n > 1; n--) {
		for(int i = 1; i < n; i++) {
			if(A[i-1] > A[i]) {
				float tmp = A[i-1];
				A[i-1] = A[i];
				A[i] = tmp;
			}
		}
	}
	if(windowSize%2==0)
		median=(A[(windowSize-2)/2]+A[windowSize/2])/2;
	if(windowSize%2==1)
		median=A[(windowSize-1)/2];
	return 0;
}



//////////////////////////////////////////////////////////////
//
// Test Driver
//
// Do not remove the #ifndef and its associated #endif
// This is required so that when you submit your code,
// this test driver will be ignored by Marmoset
//

#ifndef MARMOSET_TESTING

int main(const int argc, const char* const argv[]) {
  int size = 5;
  float dataset[] = {1, 2, 3, 4, 5};
  int windowSize = 2;
  int currentSample = 0;
  float min;
  float avg;
  float max;
  float smplSD;
  float median;

  cout << "Sample \t Minimum \t Average \t Median \t Maximum \t Sample Standard Deviation" << endl;

  while (currentSample < size) {
    int retCode = SWStats(dataset, size, currentSample, windowSize, min, avg, max, smplSD, median);
    if (retCode >= 0) {
      cout << currentSample << "\t " << min << "\t " << avg << "\t " << max << "\t " << median << "\t " << smplSD << endl;
      if (retCode > 0)
	cerr << "Warning: something weird happened, but we computed stats anyway ... YMMV" << endl;
    }
    else {
      cerr << "Error: unable to compute sliding-window statistics; exiting" << endl;
      return(-1);
    }
    ++currentSample;
  }
  return 0;
}

#endif
