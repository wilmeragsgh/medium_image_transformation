#include <Rcpp.h>
#include <fstream>
#include <string>
#include <stdlib.h>  
#include <stdio.h>  
#include <math.h>  
#include <opencv2/opencv.hpp>  
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
// [[Rcpp::export]]
RcppExport SEXP negativeTransformation(SEXP f1) {
    std::string fname = Rcpp::as<std::string>(f1); 
    std::ifstream fi;
    fi.open(fname.c_str(),std::ios::in);
    IplImage* img = 0;   
    int height,width,step,channels;  
    uchar *data;  
    int i,j,k;  
    // Load image   
    img=cvLoadImage(fname.c_str(),-1);  
    if(!img)  
    {  
        printf("Could not load image file\n");  
        exit(0);  
    }  
    // acquire image info  
    height    = img->height;    
    width     = img->width;    
    step      = img->widthStep;    
    channels  = img->nChannels;  
    data      = (uchar *)img->imageData;  
    // reverse image 
    #pragma omp parallel for collapse(3)
    for(i=0;i<height;i++)   
        for(j=0;j<width;j++)   
            for(k=0;k<channels;k++)  
                data[i*step+j*channels+k]=255-data[i*step+j*channels+k];
    //namedWindow("test.bmp", CV_WINDOW_AUTOSIZE );
    Mat im = img;
    unsigned sz = fname.size();
    fname.resize(sz-4); // removin .bmp from the name to add transformation file
    std::string transformName ("_negative");
    std::string formatName (".bmp");
    std::string outputname = fname.c_str() + transformName + formatName;
    imwrite(outputname, im);    
    //cvShowImage("mainWin", img );  
    //cvWaitKey(0);  
    //cvReleaseImage( &img );  
    Rcpp::CharacterVector rline = Rcpp::wrap(outputname);
    return rline;
}
