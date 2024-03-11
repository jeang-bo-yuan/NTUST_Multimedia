/**
 * @file main.cpp
 * @author 蔣博元
 * @brief 平滑法（box filter）和中值濾波器（mean filter）
*/
#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>

// 平滑法
cv::Mat box_filter(const cv::Mat src) {
	cv::Mat result = cv::Mat(src.rows, src.cols, CV_8UC3);

	for (int r = 0; r < src.rows; ++r) {
		for (int c = 0; c < src.cols; ++c) {
			// if on boundary, just copy
			if (r == 0 || c == 0 || r == src.rows - 1 || c == src.cols - 1) {
				result.at<cv::Vec3b>(r, c) = src.at<cv::Vec3b>(r, c);
				continue;
			}

			cv::Vec<int, 3> sum;
			for (int dr = -1; dr <= 1; ++dr) {
				for (int dc = -1; dc <= 1; ++dc) {
					cv::Vec3b color = src.at<cv::Vec3b>(r + dr, c + dc);
					sum += color;
				}
			}
			sum /= 9;

			result.at<cv::Vec3b>(r, c) = sum;
		}
	}

	return result;
}

cv::Mat mean_filter(const cv::Mat src) {
	cv::Mat result = cv::Mat(src.rows, src.cols, CV_8UC1);
	uchar grays[9];

	for (int r = 0; r < src.rows; ++r) {
		for (int c = 0; c < src.cols; ++c) {
			// if on boundary, just copy
			if (r == 0 || c == 0 || r == src.rows - 1 || c == src.cols - 1) {
				result.at<uchar>(r, c) = src.at<uchar>(r, c);
				continue;
			}

			// 將周圍九格記下來
			int i = 0;
			for (int dr = -1; dr <= 1; ++dr) {
				for (int dc = -1; dc <= 1; ++dc) {
					grays[i] = src.at<uchar>(r + dr, c + dc);
					i++;
				}
			}

			// 排序
			std::sort(grays, grays + 9);
			result.at<uchar>(r, c) = grays[4];
		}
	}

	return result;
}

int main() {
	cv::Mat img = cv::imread("Lenna.jpg");
	assert(img.type() == CV_8UC3);
	cv::imshow("Original", img);

	cv::Mat filter_result = box_filter(img);
	cv::imshow("Smooth", filter_result);
	cv::imwrite("Smooth.jpg", filter_result);

	// 加分題
	cv::Mat noise = cv::imread("lenna_noise.jpg", cv::ImreadModes::IMREAD_GRAYSCALE);
	assert(noise.type() == CV_8UC1);
	cv::imshow("With noise", noise);

	cv::Mat mean_filter_result = mean_filter(noise);
	cv::imshow("Mean Filter", mean_filter_result);
	cv::imwrite("Mean_Filter.jpg", mean_filter_result);

	cv::waitKey();
}
