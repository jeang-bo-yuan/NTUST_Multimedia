/**
 * @file main.cpp
 * @author 蔣博元
 * @brief watermark（浮水印） and scale（縮放）
*/
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src1 = imread("target/src1.png", cv::IMREAD_GRAYSCALE);	// watermark
	Mat result1 = Mat::zeros(src1.rows, src1.cols, CV_8UC1);

	//Filling your code here
	constexpr uchar mask = 0b0000'0001;
	for (int r = 0; r < src1.rows; ++r) {
		for (int c = 0; c < src1.cols; ++c) {
			result1.at<uchar>(r, c) = (src1.at<uchar>(r, c) & mask ? 255 : 0);
		}
	}

	uint scale = 2;
	Mat src2 = imread("target/src2.jpg", cv::IMREAD_GRAYSCALE);	// linear interpolation
	Mat result2 = Mat::zeros(src2.rows * scale, src2.cols * scale, CV_8UC1);
	// Filling your code here
	for (int r = 0; r < result2.rows; ++r) {
		for (int c = 0; c < result2.cols; ++c) {
			// 回推原座標
			float src_y = (float)r / scale;
			float src_x = (float)c / scale;

			float d1 = src_x - floor(src_x), d2 = 1.f - d1;
			float d3 = src_y - floor(src_y), d4 = 1.f - d3;

			int low_y = floor(src_y), high_y = min<int>(ceil(src_y), src2.rows - 1);
			int low_x = floor(src_x), high_x = min<int>(ceil(src_x), src2.cols - 1);
			float f00 = src2.at<uchar>(low_y, low_x);
			float f10 = src2.at<uchar>(low_y, high_x);
			float f01 = src2.at<uchar>(high_y, low_x);
			float f11 = src2.at<uchar>(high_y, high_x);

			float fx1 = f00 * d2 + f10 * d1;
			float fx2 = f01 * d2 + f11 * d1;
			result2.at<uchar>(r, c) = fx1 * d4 + fx2 * d3;
		}
	}

	imshow("src1", src1);
	imshow("src2", src2);
	imshow("result1", result1);
	imshow("result2", result2);
	imwrite("result1.png", result1);
	imwrite("result2.png", result2);
	/*imwrite("result.png", dst);*/
	waitKey();
}
