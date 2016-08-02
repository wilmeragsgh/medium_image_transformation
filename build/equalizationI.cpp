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
RcppExport SEXP equalizationI(SEXP f1) {
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
    Mat im = img;
    Mat im1 = im.clone();
    uchar *new_data;
    if(channels == 1){
        int histogram[256];
        // initialize all intensity values to 0
        for(int i = 0; i < 256; i++)
        {
            histogram[i] = 0;
        }
        
        // calculate the no of pixels for each intensity values
        for(int y = 0; y < height; y++)
            for(int x = 0; x < width; x++)
                histogram[(int)im.at<uchar>(y,x)]++;
        
        int size = width * height;
        float alpha = 255.0 / size;
        int cumhistogram[256];
        cumhistogram[0] = histogram[0];
        for(int i = 1; i < 256; i++)
        {
            cumhistogram[i] = histogram[i] + cumhistogram[i-1];
        }
        // Scale the histogram
        int Sk[256];
        for(int i = 0; i < 256; i++)
        {
            Sk[i] = cvRound((double)cumhistogram[i] * alpha);
        }
#pragma omp parallel for collapse(2)
        for(int y = 0; y < height; y++)
            for(int x = 0; x < width; x++)
                im1.at<uchar>(y,x) = saturate_cast<uchar>(Sk[im.at<uchar>(y,x)]);
    }
    else{
        int histogramRGB[256][3];
        // initialize all intensity values to 0
#pragma omp parallel for collapse(2)
        for(i = 0; i < 256; i++)
            for(j = 0; j < 3; j++)
                histogramRGB[i][j] = 0;
        
        // calculate the no of pixels for each intensity values
#pragma omp parallel for collapse(3)        
        for(i=0;i<height;i++) 
            for(j=0;j<width;j++)
                for(k=0;k<channels;k++)
                    histogramRGB[data[i*step+j*channels+k]][k]++;

        int size = width * height;
        float alpha = 255.0 / size;
        int cumhistogramRGB[256][3];
        cumhistogramRGB[0][0] = histogramRGB[0][0];
        cumhistogramRGB[0][1] = histogramRGB[0][1];
        cumhistogramRGB[0][2] = histogramRGB[0][2];
#pragma omp parallel for collapse(2)
        for( i = 1; i < 256; i++)
            for(j = 0;j<3;j++)
                cumhistogramRGB[i][j] = histogramRGB[i][j] + cumhistogramRGB[i-1][j];
        // Scale the histogram
        int SkRGB[256][3];
#pragma omp parallel for collapse(2)
        for( i = 0; i < 256; i++)
            for( j = 0; j < 3; j++)
                SkRGB[i][j] = cvRound((double)cumhistogramRGB[i][j] * alpha);

#pragma omp parallel for collapse(3)
        for(int y = 0; y < height; y++)
            for(int x = 0; x < width; x++)
                for(k = 0; k < channels; k++)
                im1.at<uchar>(y,x) = saturate_cast<uchar>(SkRGB[im.at<uchar>(y,x)][k]);
    }
    // reverse image 
//#pragma omp parallel for collapse(3)
  //  for(i=0;i<height;i++)   
    //    for(j=0;j<width;j++)   
      //      for(k=0;k<channels;k++)  
        //        data[i*step+j*channels+k]=255-data[i*step+j*channels+k];
    //namedWindow("test.bmp", CV_WINDOW_AUTOSIZE );
    unsigned sz = fname.size();
    fname.resize(sz-4); // removin .bmp from the name to add transformation file
    std::string transformName ("_equalized");
    std::string formatName (".bmp");
    std::string outputname = fname.c_str() + transformName + formatName;
    imwrite(outputname, im1);    
    //cvShowImage("mainWin", img );  
    //cvWaitKey(0);  
    //cvReleaseImage( &img );  
    Rcpp::CharacterVector rline = Rcpp::wrap(outputname);
    return rline;
}
