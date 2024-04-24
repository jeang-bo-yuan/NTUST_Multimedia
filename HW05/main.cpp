/**
 * @file main.cpp
 * @author 蔣博元
 * @brief Histogram Equalization 和 Histogram Matching
*/
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <assert.h>

/// 取得 src 中顏色分佈的 cdf（累積分佈函數）
/// @return 一個 1 * 256 (3 channels) 的 2D float 陣列，channel 0,1,2 分別存著 B,G,R 的 cdf，如：第 (0列, x欄) 的 channel 0,1,2 存著 B,G,R=x 時的 cdf 的值
cv::Mat get_cdf(const cv::Mat& src) {
	assert(src.type() == CV_8UC3);
	cv::Mat result = cv::Mat::zeros(1, 256, CV_32FC3);

	// count, get Histogram
	int total = 0;
	for (int r = 0; r < src.rows; ++r) {
		for (int c = 0; c < src.cols; ++c) {
			++total;
			cv::Vec3b color = src.at<cv::Vec3b>(r, c); // color from src
			result.at<cv::Vec3f>(0, color[0])[0] += 1;  // record in result
			result.at<cv::Vec3f>(0, color[1])[1] += 1;
			result.at<cv::Vec3f>(0, color[2])[2] += 1;
		}
	}

	// divide and accumulate
	for (int i = 0; i < 256; ++i) {
		result.at<cv::Vec3f>(0, i) /= total; // get probability
		if (i != 0) {
			result.at<cv::Vec3f>(0, i) += result.at<cv::Vec3f>(0, i - 1); // accumulate probability
		}
	}

	return result;
}

/// Histogram Equalization
/// @param src - 原圖片
/// @return 完成Histogram Equalization後的圖片
cv::Mat histogram_EQ(const cv::Mat& src) {
	cv::Mat cdf = get_cdf(src);
	cv::Mat result(src.rows, src.cols, CV_8UC3);

	// transform for each pixel
	for (int r = 0; r < src.rows; ++r) {
		for (int c = 0; c < src.cols; ++c) {
			cv::Vec3b src_color = src.at<cv::Vec3b>(r, c);
			cv::Vec3b& result_color = result.at<cv::Vec3b>(r, c);

			result_color[0] = cdf.at<cv::Vec3f>(0, src_color[0])[0] * 255;
			result_color[1] = cdf.at<cv::Vec3f>(0, src_color[1])[1] * 255;
			result_color[2] = cdf.at<cv::Vec3f>(0, src_color[2])[2] * 255;
		}
	}

	return result;
}

/// Histogram Match
/// @param src - 來源
/// @param target - 目標
/// @return 結果
cv::Mat histogram_match(const cv::Mat& src, const cv::Mat& target) {
	assert(src.type() == target.type() && src.type() == CV_8UC3);
	cv::Mat cdf1 = get_cdf(src);
	cv::Mat cdf2 = get_cdf(target);
	cv::Mat result(src.rows, src.cols, CV_8UC3);

	std::cout << "Histogram Match\nIn Channel -> Out\n";

	// output = cdf2^-1( cdf1(input) )
	// 建立 input 和 output 的對應表
	cv::Mat transform(1, 256, CV_8UC3);
	for (int in = 0; in < 256; ++in) {
		// for 3 channels (for B, G, R)
		for (int channel = 0; channel < 3; ++channel) {
			// y = cdf1(in)
			float y = cdf1.at<cv::Vec3f>(0, in)[channel];

			// find cdf2^-1(y)
			// 
			// y < cdf2(0)
			if (y < cdf2.at<cv::Vec3f>(0, 0)[channel]) {
				transform.at<cv::Vec3b>(0, in)[channel] = 0; // output = 0
			}
			// y > cdf2(255)
			else if (y > cdf2.at<cv::Vec3f>(0, 255)[channel]) {
				transform.at<cv::Vec3b>(0, in)[channel] = 255; // output = 255
			}
			else // find out, such that cdf2(out) is nearest to y
			for (int out = 0; out < 255; ++out) {
				// cdf2(out) <= y <= cdf2(out + 1)
				if (cdf2.at<cv::Vec3f>(0, out)[channel] <= y
					&& y <= cdf2.at<cv::Vec3f>(0, out + 1)[channel])
				{
					float d1 = y - cdf2.at<cv::Vec3f>(0, out)[channel];
					float d2 = cdf2.at<cv::Vec3f>(0, out + 1)[channel] - y;
					transform.at<cv::Vec3b>(0, in)[channel] = (d1 < d2 ? out : out + 1);
					break;
				}
			}

			std::cout << in << ' ' << channel << " -> " << (int)transform.at<cv::Vec3b>(0, in)[channel] << '\t';
		}
		std::cout << '\n';
	}

	// for each pixel
	for (int r = 0; r < result.rows; ++r) {
		for (int c = 0; c < result.cols; ++c) {
			cv::Vec3b input = src.at<cv::Vec3b>(r, c);
			cv::Vec3b& output = result.at<cv::Vec3b>(r, c);

			output[0] = transform.at<cv::Vec3b>(0, input[0])[0];
			output[1] = transform.at<cv::Vec3b>(0, input[1])[1];
			output[2] = transform.at<cv::Vec3b>(0, input[2])[2];
		}
	}

	return result;
}

int main()
{
	cv::Mat src1 = cv::imread("target/src1.jpg");
	cv::Mat result1 = histogram_EQ(src1);
	cv::Mat src2 = cv::imread("target/src2.jpg");
	cv::Mat result2 = histogram_EQ(src2);
	cv::Mat src3 = cv::imread("target/src3.jpg");
	cv::Mat target = cv::imread("target/target.jpg");
	cv::Mat result3 = histogram_match(src3, target);

	cv::imwrite("result1.jpg", result1);
	cv::imwrite("result2.jpg", result2);
	cv::imwrite("result3.jpg", result3);

	cv::imshow("src1", src1);
	cv::imshow("result1", result1);
	cv::imshow("src2", src2);
	cv::imshow("result2", result2);
	cv::imshow("src3", src3);
	cv::imshow("target", target);
	cv::imshow("result3", result3);
	cv::waitKey();
}

