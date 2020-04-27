//
// main.cpp
// opencvtestcode
//
// Created by jeffrey on 2020/4/14.
// Copyright © 2020 jeffrey. All rights reserved.
//
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;
int main( int argc, char** argv )
{
 Mat image;
 image = imread("/Users/jeffrey/aerial_view.tif", IMREAD_COLOR); // 讀取圖片
 if( image.empty() ) // Check for invalid input
 {
 std::cout << "Could not open or find the image" << std::endl;
 return -1;
 }
 cvtColor( image,image,cv::COLOR_RGB2GRAY );
 Mat grayimage;
 equalizeHist( image, grayimage );
 namedWindow( "source image", WINDOW_AUTOSIZE );
 namedWindow( "hist image", WINDOW_AUTOSIZE );//建立顯示窗口
 imshow( "source image", image );
 imshow( "hist image", grayimage );// 顯示圖片數據

 Mat srcHist, grayHist;

 int dims = 1;
 float hranges[] = {0, 255};
 const float *ranges[] = {hranges}; // 這里需要為const類型
 int size = 256;
 int channels = 0;
 calcHist(&image, 1, &channels, Mat(), srcHist, dims, &size, ranges);
 calcHist(&grayimage, 1, &channels, Mat(), grayHist, dims, &size, ranges);

 Mat srcHistImage(size, size, CV_8U, Scalar(0));
 Mat grayHistImage(size, size, CV_8U, Scalar(0));
 //獲取最大值和最小值
 double minValue = 0;
 double srcMaxValue = 0;
 double dstMaxValue = 0;
 minMaxLoc(srcHist,&minValue, &srcMaxValue, 0, 0);
 minMaxLoc(grayHist,&minValue, &dstMaxValue, 0, 0);
 //繪製出直方图
 //saturate_cast函數的作用即是：當運算完之後，結果為負，則轉為0，結果超出255，
則為255。
 int hpt = saturate_cast<int>(0.9 * size);
 for(int i = 0; i < 256; i++)
 {
 float srcValue = srcHist.at<float>(i); // 注意hist中是float
類型
 float dstValue = grayHist.at<float>(i);
 //拉伸到0-max
 int srcRealValue = saturate_cast<int>(srcValue * hpt/srcMaxValue);
 int dstRealValue = saturate_cast<int>(dstValue * hpt/dstMaxValue);

 line(srcHistImage,Point(i, size - 1),Point(i, size -
srcRealValue),Scalar(255));
 line(grayHistImage,Point(i, size - 1),Point(i, size -
dstRealValue),Scalar(255));
 }
 imshow("beforce hist", srcHistImage);
 imshow("after hist", grayHistImage);


 waitKey(0); // 等待按鍵，當你按按鍵才會繼續下段程式碼
 return 0;
}
