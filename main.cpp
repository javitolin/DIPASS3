/*
 * main.cpp
 *
 *  Created on: Apr 24, 2015
 *      Author: jdorfsman
 */

#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/operations.hpp>
#include <opencv2/core/types_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

int height;
int width;
Mat kernel, src, dst;
Point anchor;
double delta;
int ddepth;
vector<float> extractNumbers(string line, char delimeter) {
	vector<float> a;
	string forNow = "";
	for (unsigned int i = 0; i < line.size(); i++) {
		if (line[i] == delimeter) {
			std::stringstream ss;
			ss << forNow;
			float f;
			ss >> f;
			a.push_back(f);
			forNow = "";
		} else {
			forNow += line[i];
			//cout << forNow << endl;
		}
	}
	std::stringstream ss;
	ss << forNow;
	float f;
	ss >> f;
	a.push_back(f);
	forNow = "";
	//cout << f << endl;
	return a;
}

int main(int argc, char * argv[]) {
	vector<vector<float> > fromFile;
	/*DELETE*/
	vector<float> firstRow;
	firstRow.push_back(-1);
	firstRow.push_back(-2);
	firstRow.push_back(-1);
	vector<float> secondRow;
	secondRow.push_back(0);
	secondRow.push_back(0);
	secondRow.push_back(0);
	vector<float> thirdRow;
	thirdRow.push_back(1);
	thirdRow.push_back(2);
	thirdRow.push_back(1);
	fromFile.push_back(firstRow);
	fromFile.push_back(secondRow);
	fromFile.push_back(thirdRow);
	/*DELETE*/
	string line;
	if (argc < 3) {
		cout << "Wrong usage" << endl;
	}
	ifstream myfile;
	myfile.open(argv[1]);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			//cout << line << endl;
			vector<float> firstLine = extractNumbers(line, ' ');
			//cout << firstLine[0] << " " << firstLine[1] << endl;
		}
		myfile.close();
	}
	height = 3;
	width = 3;
	src = imread(argv[2], 1);
	anchor = Point(-1, -1);
	delta = 0;
	ddepth = -1;
	kernel = Mat::ones(height, width, CV_32F);
	int sum = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			kernel.at<float>(i, j) = fromFile[i][j];
			sum += fromFile[i][j];
		}
	}
	if (sum != 0) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				kernel.at<float>(i, j) = kernel.at<float>(i, j) / sum;
			}
		}
	}
	filter2D(src, dst, ddepth, kernel, anchor, delta, BORDER_DEFAULT);
	if (argc == 4) {
		cout << "Writing to file " << argv[3] << endl;
		imwrite(argv[3], dst);
	} else {
		namedWindow("Source Image", CV_WINDOW_AUTOSIZE);
		imshow("Source Image", src);
		namedWindow("Dst Image", CV_WINDOW_AUTOSIZE);
		imshow("Dst Image", dst);
		waitKey(0);
	}
	return 0;
}
