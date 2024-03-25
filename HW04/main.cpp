/**
 * @file main.cpp
 * @author 蔣博元
 * @brief 型態學之erode + connected component
*/
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("image.jpg", cv::IMREAD_GRAYSCALE);
	Mat result = Mat::zeros(src.rows, src.cols, CV_8UC1);
	resize(src, src, Size(700, (int)(700.0*src.rows/src.cols)));

	// fill your code here
	cv::threshold(src, result, 40, 255, cv::THRESH_BINARY_INV);
	cv::medianBlur(result, result, 3);

	// 25 * 25 erode
	// cv::morphologyEx(result, result, cv::MORPH_ERODE, cv::getStructuringElement(cv::MORPH_RECT, Size(25, 25)));
	cv::Mat erode_result(result.rows, result.cols, CV_8UC1);
	// for each row and col
	for (int r = 0; r < result.rows; ++r) {
		for (int c = 0; c < result.cols; ++c) {
			// 別填0
			bool not_erode = true;
			// 以(r, c)為中心看一個25 * 25的方塊
			for (int dr = -12; not_erode && dr <= 12; ++dr) {
				for (int dc = -12; not_erode && dc <= 12; ++dc) {
					int n_r = r + dr, n_c = c + dc;

					// 若at(n_r, n_c) == 0，則應該填0，也就是not_erode = false
					not_erode = !(n_r >= 0 && n_r < result.rows
						&& n_c >= 0 && n_c < result.cols
						&& result.at<uchar>(n_r, n_c) == 0);
				}
			}

			erode_result.at<uchar>(r, c) = (not_erode ? 255 : 0);
		}
	}
	result = erode_result;

	// n -> 有幾個connected component（含背景）
	Mat labels;
	int n = cv::connectedComponents(result, labels);
	std::cout << "\n\tThere are " << n - 1 << " chesses.\n" << std::endl;
	
	imshow("src", src);
	imshow("result", result);
	imwrite("result.png",result);
	waitKey();
}


