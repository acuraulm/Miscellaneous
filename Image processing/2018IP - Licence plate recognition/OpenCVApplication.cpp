// OpenCVApplication.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "common.h"
#include <queue>
#include <random>
Mat objects[9999];
Mat shrinkedMasks[9999];
int label = 0;

void setPadding(Mat src, int val) {
	int height = src.rows;
	int width = src.cols;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++) {
			if (i < 5 || j < 55 || i > src.rows-5 || j > src.cols-5) {
				src.at<uchar>(i, j) = val;
			}
		}
}
Mat negativeOf(Mat src)
{
		int height = src.rows;
		int width = src.cols;
		Mat dst = src.clone();
										   // the fastest approach using the “diblook style”
		uchar *lpSrc = src.data;
		uchar *lpDst = dst.data;
		int w = (int)src.step; // no dword alignment is done !!!
		for (int i = 0; i<height; i++)
			for (int j = 0; j < width; j++) {
				uchar val = lpSrc[i*w + j];
				lpDst[i*w + j] = 255 - val;
			}
	//	imshow("input image", src);
	//	imshow("negative image", dst);
		
		return dst;
}
typedef struct point {
	int i;
	int j;
};
Mat cropImage(Mat src) {
	std::vector<std::vector<cv::Point> > contour;
	Mat dst = Mat(src.rows, src.cols, CV_8UC1);
	findContours(src, contour, RETR_TREE, CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	drawContours(dst, contour, 1, 0, 1, 8, noArray());
	//std::cout << contour[2];
	imshow("DST", dst);
	for (int i = 0; i<contour.size(); i++) {
		for (int j = 0; j<contour[0].size(); j++) {
				//newArr[contour.size() * i + j] = contour[i][j];
			std::cout << contour[i][j];
		}
		std::cout << std::endl;
	}
	for (int i = 0; i < contour[0].size(); i++) {
		std::cout << contour[0][i];
	}
	return src;
}

typedef struct borderPoint {
	int i;
	int j;
	int dir;
}borderP;

void setExtremePoints(std::vector<borderP> array, Point lU, Point lB, Point rU, Point rB) {

	for (int i = 0; i < array.size(); i++) {
		;
	}
}
Mat borderTracing(Mat src) {
		int height = src.rows;
		int width = src.cols;
			Mat dst = Mat(height, width, CV_8UC3);
			std::vector<int> absoluteC, derivativeC;
			std::vector<borderP> borders;
			borderP P0, P1, Pn, Pn1;
			P0.i = 0;
			P0.j = 0;
			P0.dir = 0;
			int offsetI[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };
			int offsetJ[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					if (src.at<uchar>(i, j) == 0) {
						P0.i = i;
						P0.j = j;
						P0.dir = 7;
						i = height + 1;
						j = width + 1;
						borders.push_back(P0);
					}
				}
			}
			std::cout << " P0: " << P0.i << "," << P0.j << std::endl;
			do {
				borderP currentPoint = borders.at(borders.size() - 1);
				if ((currentPoint.dir % 2))
					currentPoint.dir = (currentPoint.dir + 6) % 8;
				else
					currentPoint.dir = (currentPoint.dir + 7) % 8;
				std::vector<borderP> neighbors;
				neighbors.resize(8);
				for (int i = 0; i < 8; i++) {
					neighbors.at(i) = borderP{ currentPoint.i + offsetI[i], currentPoint.j + offsetJ[i], i };
				}
				while (src.at<uchar>(neighbors.at(currentPoint.dir).i, neighbors.at(currentPoint.dir).j) != 0) {
					currentPoint.dir = (currentPoint.dir + 1) % 8;
				}

				dst.at<Vec3b>(neighbors.at(currentPoint.dir).i, neighbors.at(currentPoint.dir).j) = Vec3b(0, 0, 0);
				borders.push_back(borderP{ neighbors.at(currentPoint.dir).i , neighbors.at(currentPoint.dir).j , currentPoint.dir });

				std::cout << currentPoint.dir << " ";
				P1 = borders.at(1);
				Pn1 = borders.at(borders.size() - 2);
				Pn = borders.at(borders.size() - 1);
				//std::cout << "Size:" << borders.size();
			} while (!((borders.size() > 2) && (Pn.i == P1.i && Pn.j == P1.j) && (Pn1.i == P0.i && Pn1.j == P0.j)));

			std::vector<Point> contour;
			for (int i = 0; i < borders.size(); i++) {
				contour.push_back(Point(borders[i].i, borders[i].j));
			}
			/*for (int i = 0; i < borders.size(); i++) {
				std::cout << "["<< borders[i].i <<", "<< borders[i].j << "] ";
			}

			for (int i = 0; i < contour.size(); i++) {
				std::cout << "[" << contour[i].x << ", " << contour[i].y << "] ";
			}
			*/
			std::cout << "Area:" << contourArea(contour);
			//std::cout <<"Perimeter:" << cvContourPerimeter(contour.a);
			//Mat outputA = src(Rect(boundingRect(contour).x + 20, boundingRect(contour).y - 20, boundingRect(contour).height, boundingRect(contour).width));
			Mat outputA = Mat(boundingRect(contour).width, boundingRect(contour).height, CV_8UC1);

			imshow("ceva", dst);

			imshow("bounding", outputA);

			return outputA;
	
}
boolean borderTracingBool(Mat src) {
	int height = src.rows;
	int width = src.cols;
	Mat dst = Mat(height, width, CV_8UC3);
	std::vector<int> absoluteC, derivativeC;
	std::vector<borderP> borders;
	borderP P0, P1, Pn, Pn1;
	P0.i = 0;
	P0.j = 0;
	P0.dir = 0;
	int offsetI[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };
	int offsetJ[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (src.at<uchar>(i, j) == 0) {
				P0.i = i;
				P0.j = j;
				P0.dir = 7;
				i = height + 1;
				j = width + 1;
				borders.push_back(P0);
			}
		}
	}
	//std::cout << " P0: " << P0.i << "," << P0.j << std::endl;
	do {
		borderP currentPoint = borders.at(borders.size() - 1);
		if ((currentPoint.dir % 2))
			currentPoint.dir = (currentPoint.dir + 6) % 8;
		else
			currentPoint.dir = (currentPoint.dir + 7) % 8;
		std::vector<borderP> neighbors;
		neighbors.resize(8);
		for (int i = 0; i < 8; i++) {
			neighbors.at(i) = borderP{ currentPoint.i + offsetI[i], currentPoint.j + offsetJ[i], i };
		}
		while (src.at<uchar>(neighbors.at(currentPoint.dir).i, neighbors.at(currentPoint.dir).j) != 0) {
			currentPoint.dir = (currentPoint.dir + 1) % 8;
		}

		dst.at<Vec3b>(neighbors.at(currentPoint.dir).i, neighbors.at(currentPoint.dir).j) = Vec3b(0, 0, 0);
		borders.push_back(borderP{ neighbors.at(currentPoint.dir).i , neighbors.at(currentPoint.dir).j , currentPoint.dir });

		//std::cout << currentPoint.dir << " ";
		P1 = borders.at(1);
		Pn1 = borders.at(borders.size() - 2);
		Pn = borders.at(borders.size() - 1);
		//std::cout << "Size:" << borders.size();
	} while (!((borders.size() > 2) && (Pn.i == P1.i && Pn.j == P1.j) && (Pn1.i == P0.i && Pn1.j == P0.j)));

	std::vector<Point> contour;
	for (int i = 0; i < borders.size(); i++) {
		contour.push_back(Point(borders[i].i, borders[i].j));
	}
	/*for (int i = 0; i < borders.size(); i++) {
	std::cout << "["<< borders[i].i <<", "<< borders[i].j << "] ";
	}

	for (int i = 0; i < contour.size(); i++) {
	std::cout << "[" << contour[i].x << ", " << contour[i].y << "] ";
	}
	*/
	//std::cout << "Area:" << contourArea(contour);
	//std::cout <<"Perimeter:" << cvContourPerimeter(contour.a);
	//Mat outputA = src(Rect(boundingRect(contour).x + 20, boundingRect(contour).y - 20, boundingRect(contour).height, boundingRect(contour).width));
	Mat outputA = Mat(boundingRect(contour).width, boundingRect(contour).height, CV_8UC1);
	//if ((float)outputA.rows / (float)outputA.cols >= 3.5 && (float)outputA.rows / (float)outputA.cols <= 6.5
	if((float)outputA.cols / (float)outputA.rows >= 2.5 && contourArea(contour) > 500) {
		//std::cout << "AICI AJUNG ";
		return true;
	}
	else
		return false;

}
Mat labelEquivalence(Mat src) {
	int height = src.rows;
	int width = src.cols;
	Mat labels(height, width, CV_32SC1, Scalar(0));
	label = 0;

	for (int i = 1; i < height - 1; i++)
	{
		for (int j = 1; j < width - 1; j++)
		{
			if (src.at<uchar>(i, j) == 0 && labels.at<int>(i, j) == 0) {
				label++;
				std::queue<Point2i> Q;
				labels.at<int>(i, j) = label;
				Q.push(Point2i(j, i));
				while (!Q.empty()) {
					Point2i q = Q.front();
					Q.pop();
					for (int rowz = q.y - 1; rowz <= q.y + 1; rowz++)
						for (int colz = q.x - 1; colz <= q.x + 1; colz++)
							if (src.at<uchar>(rowz, colz) == 0 && labels.at<int>(rowz, colz) == 0) {
								labels.at<int>(rowz, colz) = label;
								Q.push(Point2i(colz, rowz));
							}
				}
			}
		}
	}
	std::default_random_engine gen;
	std::uniform_int_distribution<int> d(0, 255);
	uchar x = d(gen);
	//Mat *objects = new Mat[label];
	
	for (int k = 1; k <= label; k++) {
		objects[k] = Mat(height, width, CV_8UC1);
	}
	std::cout << "Labels: " << label;
	Mat dst(height, width, CV_8UC1, 255);
	for (int i = 1; i < height - 1; i++)
	{
		for (int j = 1; j < width - 1; j++)
		{
			for (int k = 1; k <= label; k++)
				if (labels.at<int>(i, j) == k) {
					dst.at<uchar>(i, j) = 0;
					objects[k].at<uchar>(i, j) = 0;;
				}
				else
					objects[k].at<uchar>(i, j) = 255;

		}
	}
	

	//	imshow("input image", src);
	//	imshow("colored labels", dst);

		return dst;
}



Mat tresholding(Mat src) {
	int height = src.rows;
	int width = src.cols;
	Mat dst = Mat(height, width, CV_8UC1);
	int minIntensity = 0;
	int maxIntensity = 255;

	int h[256] = { 0 };
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			h[src.at<uchar>(i, j)]++;
		}
	}
	for (int i = 0; i < 256; i++) {
		if (h[i] > 0) {
			minIntensity = i;
			i = 255;
		}
	}
	for (int i = 255; i >= 0; i--) {
		if (h[i] > 0) {
			maxIntensity = i;
			i = 0;
		}
	}

	int value = (minIntensity + maxIntensity) / 2;

	for (int i = 0; i<height; i++)
	{
		for (int j = 0; j<width; j++)
		{
			uchar val = src.at<uchar>(i, j);
			if (val < value)
				dst.at<uchar>(i, j) = 0;
			else
				dst.at<uchar>(i, j) = 255;
		}
	}

	return dst;
}

Mat preProcessing(Mat src, char *type) {
	//	imshow("Grayscale", src);
		int height = src.rows;
		int width = src.cols;
		Mat thresholded = tresholding(src);
	//	imshow("Threshold", thresholded);
		Mat kernel = Mat(5, 5, CV_8UC1);
		for(int i=0;i<5;i++)
			for (int j = 0; j < 5; j++) {
				kernel.at<uchar>(i, j) = 1;
			}

		Mat openingThresholded = Mat(height, width, CV_8UC1);
		Mat cannyInput = Mat(height, width, CV_8UC1);
		Mat cannyThresholded = Mat(height, width, CV_8UC1);
		Mat cannyOpening = Mat(height, width, CV_8UC1);

		morphologyEx(thresholded, openingThresholded, MORPH_OPEN, kernel);
	//	imshow("Input preProcessing", openingThresholded);
		
		Canny(thresholded, cannyThresholded, 150, 450, 3);
		Canny(openingThresholded, cannyOpening, 150, 450, 3);
		imshow("Canny", cannyOpening);
		Canny(src, cannyInput, 150, 450, 3);

		if (strcmp(type, "opening")== 0) {
			std::cout << "OPENING\n";
			return openingThresholded;
		}
		else if (strcmp(type, "cannyo")== 0) {
			std::cout << "CANNY OPENING\n";
			return cannyOpening;
		}
		else if (strcmp(type, "cannyt")==0) {
			std::cout << "CANNY BW\n";
			return cannyThresholded;
		}
		else if (strcmp(type, "cannyi")==0) {
			std::cout << "CANNY SOURCE\n";
			return cannyInput;
		}

		return thresholded;
}
int nrOfLabelsInside(Mat src) {
	int height = src.rows;
	int width = src.cols;
	Mat labels(height, width, CV_32SC1, Scalar(0));
	int labelB = 0;

	for (int i = 1; i < height - 1; i++)
	{
		for (int j = 1; j < width - 1; j++)
		{
			if (src.at<uchar>(i, j) == 0 && labels.at<int>(i, j) == 0) {
				labelB++;
				std::queue<Point2i> Q;
				labels.at<int>(i, j) = labelB;
				Q.push(Point2i(j, i));
				while (!Q.empty()) {
					Point2i q = Q.front();
					Q.pop();
					for (int rowz = q.y - 1; rowz <= q.y + 1; rowz++)
						for (int colz = q.x - 1; colz <= q.x + 1; colz++)
							if (src.at<uchar>(rowz, colz) == 0 && labels.at<int>(rowz, colz) == 0) {
								labels.at<int>(rowz, colz) = labelB;
								Q.push(Point2i(colz, rowz));
							}
				}
			}
		}
	}
	return labelB;
}

void drawRectangle(Mat from, Mat to) {
	int height = from.rows;
	int width = from.cols;
	for (int i = 0; i<height; i++)
	{
		for (int j = 0; j<width; j++)
		{
			uchar val = from.at<uchar>(i, j);
			if (val == 0)
				to.at<Vec3b>(i, j) = Vec3b(0, 255, 0);
		}
	}
}
int main()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{

		Mat src;
		Mat src2 = imread(fname, CV_LOAD_IMAGE_COLOR);
		imshow("Input", src2);
		cvtColor(src2, src, COLOR_RGB2GRAY);
		// Pre-Processing
		Mat processedOpening = preProcessing(src, "opening");
		imshow("Pre-processing", processedOpening);
		setPadding(processedOpening, 0);
		// Shape-Recognition
		Mat labeledObjects = labelEquivalence(negativeOf(processedOpening));
		imshow("Labels", labeledObjects);

		//simshow("Object 23", objects[5]);
		//cvtColor(objects[23], newObj, COLOR_RGB2GRAY);
	//	imshow("new obj.bmp", objects[23]);
	//	borderTracing(objects[23]);
		
		//Mat output;
		//cvtColor(src, output, COLOR_GRAY2RGB);
		for (int k = 1; k <= label; k++) {
		//	imshow("Label", objects[k]);
		//	waitKey(250);
			if (borderTracingBool(objects[k])) {
				
				if (nrOfLabelsInside(negativeOf(objects[k])) > 5 && nrOfLabelsInside(negativeOf(objects[k])) < 12) {
					drawRectangle(objects[k], src2);
					
				}
			}
		}
		
		imshow("Output", src2);
		waitKey(0);

	}
	return 0;
}