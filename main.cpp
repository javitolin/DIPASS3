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


vector<float> extractNumbers(string line, char delimiter) {
	vector<float> a;
	float f;
	string forNow = "";
	for (unsigned int i = 0; i < line.size(); i++) {
		if (line.at(i) == delimiter) {
			f = ::atof(forNow.c_str());
			//cout << ::atof(forNow.c_str()) << endl;
			a.push_back(f);
			forNow = "";
		} else {
			forNow += line.at(i);
		}
	}
	f = ::atof(forNow.c_str());
	//cout << f << endl;
	a.push_back(f);
	return a;
}

int main(int argc, char * argv[]) {
	bool firstLine = true;
	vector<vector<float> > fromFile;
	string line;
	if (argc < 3) {
		cout << "Usage: ApplyFilter input_image_file filter_file output_image_file" << endl;
		cin.get();
		return -1;
	}
	string filterFile = argv[2];
	string imageFile  = argv[1];
	/*Load File*/
	ifstream myfile;
	myfile.open(filterFile);
	if (myfile.is_open()) {
		while (!myfile.eof()) {
			getline(myfile,line);
			vector<float> floatLine = extractNumbers(line, ' ');
			if(firstLine){
				if(floatLine.size() != 2){
					continue;
					cout << "Error: First Line must be width height only!" << endl;
					cin.get();
					return -1;
				}
				firstLine = false;
				height = floatLine[0];
				width = floatLine[1];
			}
			else{
				if(floatLine.size() != width){
					cout << "Error: Matrix is not in the size of specified width!" << endl;
					cin.get();
					return -1;
				}
				fromFile.push_back(floatLine);
			}
		}
		myfile.close();
	}
	if(fromFile.size() != width){
		cout << "Error: Matrix is not in the size of specified Height!" << endl;
		cin.get();
		return -1;
	}
	cout << height << " " << width << endl;
	/*Read Image*/
	src = imread(imageFile, 1);
	/*Init Vars*/
	anchor = Point(-1, -1);
	delta = 0;
	ddepth = -1;
	
	/*Create Filter matrix*/
	kernel = Mat::ones(height, width, CV_32F);
	int sum = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			kernel.at<float>(i, j) = fromFile[i][j];
			sum += fromFile[i][j];
		}
	}
	
	/*Normalize Matrix*/
	if (sum != 0) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				kernel.at<float>(i, j) = kernel.at<float>(i, j) / sum;
			}
		}
	}
	
	/*Apply Filter*/
	filter2D(src, dst, ddepth, kernel, anchor, delta, BORDER_DEFAULT);
	
	
	if (argc == 4) {	/*Write to file.*/
		cout << "Writing to file " << argv[3] << endl;
		imwrite(argv[3], dst);
	} else {			/*Show on screen.*/
		namedWindow("Source Image", CV_WINDOW_AUTOSIZE);
		imshow("Source Image", src);
		namedWindow("Dst Image", CV_WINDOW_AUTOSIZE);
		imshow("Dst Image", dst);
		waitKey(0);
	}
	return 0;
}
