#include <Rcpp.h>
#include <fstream>
#include <string>
#include <stdlib.h>  
#include <stdio.h>  
#include <math.h>  
#include <opencv2/opencv.hpp>  
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;
// [[Rcpp::export]]
RcppExport SEXP rotateTransformation(SEXP f1, int n) {
    std::string fname = Rcpp::as<std::string>(f1); 
    std::ifstream fi;
    fi.open(fname.c_str(),std::ios::in);
    IplImage* img = 0;
    int height,width,step,channels;  
    uchar *data;
    uchar *data1;
    int i,j,k;  
    // Load image   
    img=cvLoadImage(fname.c_str(),-1);  
    if(!img)  
    {  
        printf("Could not load image file\n");  
        exit(0);  
    }  
    // acquire image info  
    height = img->height;    
    width = img->width;    
    step  = img->widthStep;
    channels = img->nChannels;  
    data = (uchar *)img->imageData;
    // reverse image 
    int round;
    IplImage* img1;
    #pragma omp parallel for collapse(3)
    for(round=0;round<n;round++){
        img1 = img;
        data1 = (uchar *)img1->imageData;
        int step1 = abs(width - height);
        for(i=0;i<width;i++)   
            for(j=0;j<height;j++)   
                for(k=0;k<channels;k++)  
                    data[i*step1+j*channels+k]=data1[(height - 1 - j)*step+(width - (channels * (i + 1))) ];
    }
    int temp = img->height;
    img->height = width;
    img->width = temp;
    Mat im = img;
    unsigned sz = fname.size();
    fname.resize(sz-4); // removin .bmp from the name to add transformation file
    std::string transformName ("_mirrorVer");
    std::string formatName (".bmp");
    std::string outputname = fname.c_str() + transformName + formatName;
    imwrite(outputname, im);    
    //cvShowImage("mainWin", img );  
    //cvWaitKey(0);  
    //cvReleaseImage( &img );  
    Rcpp::CharacterVector rline = Rcpp::wrap(outputname);
    return rline;
}
