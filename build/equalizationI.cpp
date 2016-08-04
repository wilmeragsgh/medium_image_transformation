#include <Rcpp.h>
#include <fstream>
#include <string>
#include <stdlib.h>  
#include <stdio.h>  
#include <math.h>  
#include <opencv2/opencv.hpp>  
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
static float Min(float a, float b) {
    return a <= b ? a : b;
}

static float Max(float a, float b) {
    return a >= b ? a : b;
}

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
    uchar *aux_to_ycbcr;
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
        int histogramY[256];
        // initialize all intensity values to 0
#pragma omp parallel for collapse(1)
        for(i = 0; i < 256; i++)
                histogramY[i] = 0;
        
        // calculate the no of pixels for each intensity values
#pragma omp parallel for collapse(3)        
        for(i=0;i<height;i++){
            for(j=0;j<width;j++){
                aux_to_ycbcr[i*step+j*channels + 0] = (float)(0.2989 * (data[i*step+j*channels+0]/255) + 0.5866 * (data[i*step+j*channels+1]/255) + 0.1145 * (data[i*step+j*channels+2]/255)); // Y channel
                aux_to_ycbcr[i*step+j*channels + 1] = (float)(-0.1687 * (data[i*step+j*channels+0]/255) - 0.3313 * (data[i*step+j*channels+1]/255) + 0.5000 * (data[i*step+j*channels+2]/255)); // Cb channel
                aux_to_ycbcr[i*step+j*channels + 2] = (float)(0.5000 * (data[i*step+j*channels+0]/255) - 0.4184 * (data[i*step+j*channels+1]/255) - 0.0816 * (data[i*step+j*channels+2]/255)); // Cr channel
                histogramY[aux_to_ycbcr[i*step+j*channels + 0]]++;
            }
        }

        int size = width * height;
        float alpha = 255.0 / size;
        int cumhistogramY[256];
        cumhistogramY[0] = histogramY[0];
        
#pragma omp parallel for collapse(1)
        for( i = 1; i < 256; i++)
                cumhistogramY[i] = histogramY[i] + cumhistogramY[i-1];
        // Scale the histogram
        int SkY[256];
#pragma omp parallel for collapse(1)
        for( i = 0; i < 256; i++)
            SkY[i] = cvRound((double)cumhistogramY[i] * alpha);

        uchar new_y;
#pragma omp parallel for collapse(2)
        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                new_y = saturate_cast<uchar>(SkY[aux_to_ycbcr[i*step+j*channels + 0]]);
                im1.at<Vec3b>(y, x)[0] = (unsigned char)(255 * Max(0.0f, Min(1.0f, (float)(new_y + 0.0000 * aux_to_ycbcr[i*step+j*channels + 1] + 1.4022 * aux_to_ycbcr[i*step+j*channels + 2]))));
                im1.at<Vec3b>(y, x)[1] = (unsigned char)(255 * Max(0.0f, Min(1.0f, (float)(new_y - 0.3456 * aux_to_ycbcr[i*step+j*channels + 1] - 0.7145 * aux_to_ycbcr[i*step+j*channels + 2]))));
                im1.at<Vec3b>(y, x)[2] = (unsigned char)(255 * Max(0.0f, Min(1.0f, (float)(new_y + 1.7710 * aux_to_ycbcr[i*step+j*channels + 1] + 0.0000 * aux_to_ycbcr[i*step+j*channels + 2]))));
            }
        }
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
