/**
 * @file main.cpp
 * @author 蔣博元
 * @brief 將圖片轉灰階
*/
#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
	cv::Mat img = cv::imread("Lenna.jpg");
	assert(img.type() == CV_8UC3);

	for (int row = 0; row < img.rows; ++row) {
		for (int col = 0; col < img.cols; ++col) {
			uchar* color = img.ptr(row, col);

			uchar gray = 0.114 * color[0] + 0.587 * color[1] + 0.299 * color[2];
			color[0] = color[1] = color[2] = gray;
		}
	}


	cv::imshow("Hello World!", img);
	cv::waitKey();
}
