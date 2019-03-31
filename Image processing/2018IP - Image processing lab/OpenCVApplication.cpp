// OpenCVApplication.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "common.h"
#include <queue>
#include <random>

void testOpenImage()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		imshow("image",src);
		waitKey();
	}
}
void testOpenImagesFld()
{
	char folderName[MAX_PATH];
	if (openFolderDlg(folderName)==0)
		return;
	char fname[MAX_PATH];
	FileGetter fg(folderName,"bmp");
	while(fg.getNextAbsFile(fname))
	{
		Mat src;
		src = imread(fname);
		imshow(fg.getFoundFileName(),src);
		if (waitKey()==27) //ESC pressed
			break;
	}
}
void testImageOpenAndSave()
{
	Mat src, dst;

	src = imread("Images/Lena_24bits.bmp", CV_LOAD_IMAGE_COLOR);	// Read the image
	
	if (!src.data)	// Check for invalid input
	{
		printf("Could not open or find the image\n");
		return;
	}

	// Get the image resolution
	Size src_size = Size(src.cols, src.rows);

	// Display window
	const char* WIN_SRC = "Src"; //window for the source image
	namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_SRC, 0, 0);

	const char* WIN_DST = "Dst"; //window for the destination (processed) image
	namedWindow(WIN_DST, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_DST, src_size.width + 10, 0);

	cvtColor(src, dst, CV_BGR2GRAY); //converts the source image to a grayscale one

	imwrite("Images/Lena_24bits_gray.bmp", dst); //writes the destination to file

	imshow(WIN_SRC, src);
	imshow(WIN_DST, dst);

	printf("Press any key to continue ...\n");
	waitKey(0);
}
void testNegativeImage()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		double t = (double)getTickCount(); // Get the current time [s]
		
		Mat src = imread(fname,CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height,width,CV_8UC1);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i=0; i<height; i++)
		{
			for (int j=0; j<width; j++)
			{
				uchar val = src.at<uchar>(i,j);
				uchar neg = 255 - val;
				dst.at<uchar>(i,j) = neg;
			}
		}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image",src);
		imshow("negative image",dst);
		waitKey();
	}
}
void testParcurgereSimplaDiblookStyle()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = src.clone();
		
		double t = (double)getTickCount(); // Get the current time [s]

		// the fastest approach using the “diblook style”
		uchar *lpSrc = src.data;
		uchar *lpDst = dst.data;
		int w = (int) src.step; // no dword alignment is done !!!
		for (int i = 0; i<height; i++)
			for (int j = 0; j < width; j++) {
				uchar val = lpSrc[i*w + j];
				lpDst[i*w + j] = 255 - val;
			}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image",src);
		imshow("negative image",dst);
		imwrite("blabla3.bmp", dst);
		waitKey();
	}
}
void testColor2Gray()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src = imread(fname);

		int height = src.rows;
		int width = src.cols;

		Mat dst = Mat(height,width,CV_8UC1);

		// Asa se acceseaaza pixelii individuali pt. o imagine RGB 24 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i=0; i<height; i++)
		{
			for (int j=0; j<width; j++)
			{
				Vec3b v3 = src.at<Vec3b>(i,j);
				uchar b = v3[0];
				uchar g = v3[1];
				uchar r = v3[2];
				dst.at<uchar>(i,j) = (r+g+b)/3;
			}
		}
		
		imshow("input image",src);
		imshow("gray image",dst);
		waitKey();
	}
}
void testBGR2HSV()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname);
		int height = src.rows;
		int width = src.cols;

		// Componentele d eculoare ale modelului HSV
		Mat H = Mat(height, width, CV_8UC1);
		Mat S = Mat(height, width, CV_8UC1);
		Mat V = Mat(height, width, CV_8UC1);

		// definire pointeri la matricele (8 biti/pixeli) folosite la afisarea componentelor individuale H,S,V
		uchar* lpH = H.data;
		uchar* lpS = S.data;
		uchar* lpV = V.data;

		Mat hsvImg;
		cvtColor(src, hsvImg, CV_BGR2HSV);

		// definire pointer la matricea (24 biti/pixeli) a imaginii HSV
		uchar* hsvDataPtr = hsvImg.data;

		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				int hi = i*width * 3 + j * 3;
				int gi = i*width + j;

				lpH[gi] = hsvDataPtr[hi] * 510 / 360;		// lpH = 0 .. 255
				lpS[gi] = hsvDataPtr[hi + 1];			// lpS = 0 .. 255
				lpV[gi] = hsvDataPtr[hi + 2];			// lpV = 0 .. 255
			}
		}

		imshow("input image", src);
		imshow("H", H);
		imshow("S", S);
		imshow("V", V);

		waitKey();
	}
}
void testResize()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		Mat dst1,dst2;
		//without interpolation
		resizeImg(src,dst1,320,false);
		//with interpolation
		resizeImg(src,dst2,320,true);
		imshow("input image",src);
		imshow("resized image (without interpolation)",dst1);
		imshow("resized image (with interpolation)",dst2);
		waitKey();
	}
}
void testCanny()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src,dst,gauss;
		src = imread(fname,CV_LOAD_IMAGE_GRAYSCALE);
		double k = 0.4;
		int pH = 50;
		int pL = (int) k*pH;
		GaussianBlur(src, gauss, Size(5, 5), 0.8, 0.8);
		Canny(gauss,dst,pL,pH,3);
		imshow("input image",src);
		imshow("canny",dst);
		waitKey();
	}
}
void testVideoSequence()
{
	VideoCapture cap("Videos/rubic.avi"); // off-line video from file
	//VideoCapture cap(0);	// live video from web cam
	if (!cap.isOpened()) {
		printf("Cannot open video capture device.\n");
		waitKey(0);
		return;
	}
		
	Mat edges;
	Mat frame;
	char c;

	while (cap.read(frame))
	{
		Mat grayFrame;
		cvtColor(frame, grayFrame, CV_BGR2GRAY);
		Canny(grayFrame,edges,40,100,3);
		imshow("source", frame);
		imshow("gray", grayFrame);
		imshow("edges", edges);
		c = cvWaitKey(0);  // waits a key press to advance to the next frame
		if (c == 27) {
			// press ESC to exit
			printf("ESC pressed - capture finished\n"); 
			break;  //ESC pressed
		};
	}
}
void testSnap()
{
	VideoCapture cap(0); // open the deafult camera (i.e. the built in web cam)
	if (!cap.isOpened()) // openenig the video device failed
	{
		printf("Cannot open video capture device.\n");
		return;
	}

	Mat frame;
	char numberStr[256];
	char fileName[256];
	
	// video resolution
	Size capS = Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH),
		(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));

	// Display window
	const char* WIN_SRC = "Src"; //window for the source frame
	namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_SRC, 0, 0);

	const char* WIN_DST = "Snapped"; //window for showing the snapped frame
	namedWindow(WIN_DST, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_DST, capS.width + 10, 0);

	char c;
	int frameNum = -1;
	int frameCount = 0;

	for (;;)
	{
		cap >> frame; // get a new frame from camera
		if (frame.empty())
		{
			printf("End of the video file\n");
			break;
		}

		++frameNum;
		
		imshow(WIN_SRC, frame);

		c = cvWaitKey(10);  // waits a key press to advance to the next frame
		if (c == 27) {
			// press ESC to exit
			printf("ESC pressed - capture finished");
			break;  //ESC pressed
		}
		if (c == 115){ //'s' pressed - snapp the image to a file
			frameCount++;
			fileName[0] = NULL;
			sprintf(numberStr, "%d", frameCount);
			strcat(fileName, "Images/A");
			strcat(fileName, numberStr);
			strcat(fileName, ".bmp");
			bool bSuccess = imwrite(fileName, frame);
			if (!bSuccess) 
			{
				printf("Error writing the snapped image\n");
			}
			else
				imshow(WIN_DST, frame);
		}
	}

}
void MyCallBackFunc(int event, int x, int y, int flags, void* param)
{
	//More examples: http://opencvexamples.blogspot.com/2014/01/detect-mouse-clicks-and-moves-on-image.html
	Mat* src = (Mat*)param;
	if (event == CV_EVENT_LBUTTONDOWN)
		{
			printf("Pos(x,y): %d,%d  Color(RGB): %d,%d,%d\n",
				x, y,
				(int)(*src).at<Vec3b>(y, x)[2],
				(int)(*src).at<Vec3b>(y, x)[1],
				(int)(*src).at<Vec3b>(y, x)[0]);
		}
}
void testMouseClick()
{
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname);
		//Create a window
		namedWindow("My Window", 1);

		//set the callback function for any mouse event
		setMouseCallback("My Window", MyCallBackFunc, &src);

		//show the image
		imshow("My Window", src);

		// Wait until user press some key
		waitKey(0);
	}
}
/* Histogram display function - display a histogram using bars (simlilar to L3 / PI)
Input:
name - destination (output) window name
hist - pointer to the vector containing the histogram values
hist_cols - no. of bins (elements) in the histogram = histogram image width
hist_height - height of the histogram image
Call example:
showHistogram ("MyHist", hist_dir, 255, 200);
*/
void showHistogram(const std::string& name, int* hist, const int  hist_cols, const int hist_height)
{
	Mat imgHist(hist_height, hist_cols, CV_8UC3, CV_RGB(255, 255, 255)); // constructs a white image

	//computes histogram maximum
	int max_hist = 0;
	for (int i = 0; i<hist_cols; i++)
	if (hist[i] > max_hist)
		max_hist = hist[i];
	double scale = 1.0;
	scale = (double)hist_height / max_hist;
	int baseline = hist_height - 1;

	for (int x = 0; x < hist_cols; x++) {
		Point p1 = Point(x, baseline);
		Point p2 = Point(x, baseline - cvRound(hist[x] * scale));
		line(imgHist, p1, p2, CV_RGB(255, 0, 255)); // histogram bins colored in magenta
	}

	imshow(name, imgHist);
}
void additiveGrayscale(int incVal) {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		double t = (double)getTickCount(); // Get the current time [s]

		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				uchar val = src.at<uchar>(i, j);
				uchar addGray;
				if (val + incVal >= 255)
					addGray = 255;
				else if (val + incVal <= 0)
					addGray = 0;
				else
					addGray = val + incVal;
				dst.at<uchar>(i, j) = addGray;
			}
		}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image", src);
		imshow("additive image", dst);
		waitKey();
	}
}
void multiplicativeGrayscale(float mulVal) {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		double t = (double)getTickCount(); // Get the current time [s]

		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				uchar val = src.at<uchar>(i, j);
				uchar mulGray;
				if (val*mulVal >= 255)
					mulGray = 255;
				else if (val*mulVal <= 0)
					mulGray = 0;
				else
					mulGray = val*mulVal;
				dst.at<uchar>(i, j) = mulGray;
			}
		}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image", src);
		imshow("multiplicative image", dst);
		imwrite("multiplicativeImage.bmp", dst);
		waitKey();
	}
	
}	
void coloredSquares() {
	Mat img = Mat(256, 256, CV_8UC3);
	Vec3b v3;
	//First square
	v3[0] = 255;
	v3[1] = 255;
	v3[2] = 255;
	for (int i = 0; i < 128; i++)
	{
		for (int j = 0; j < 128; j++)
		{
			img.at<Vec3b>(i, j) = v3;

		}
	}
	//2nd square
	v3[0] = 0;
	v3[1] = 255;
	v3[2] = 0;
	
	for (int i = 128; i<256; i++)
	{
		for (int j = 0; j < 128; j++)
		{
			img.at<Vec3b>(i, j) = v3;

		}
	}
	//3nd square
	v3[0] = 0;
	v3[1] = 0;
	v3[2] = 255;
	for (int i = 0; i < 128; i++)
	{
		for (int j = 128; j < 256; j++)
		{
			img.at<Vec3b>(i, j) = v3;

		}
	}
	//Last square
	v3[1] = 255;
	v3[2] = 255;
	for (int i = 128; i <256; i++)
	{
		for (int j = 128; j < 256; j++)
		{
			img.at<Vec3b>(i, j) = v3;

		}
	}
	imshow("output", img);
	imwrite("Images/colorsquares.bmp", img);
	waitKey();

}
void inverseMatrix() {
	int b; float vals[9];
	for (int i = 0; i < 9; i++) {
		std::cin >> vals[i];
	}
	Mat M(3, 3, CV_32FC1, vals);
	std::cout << M.inv();
	std::cin >> b;
}
void rgbChannels() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		double t = (double)getTickCount(); // Get the current time [s]

		Mat_<Vec3b> src = imread(fname, 1);
		int height = src.rows;
		int width = src.cols;
		Mat_<Vec3b> dstB = Mat_<Vec3b>(height, width, CV_8UC3);
		Mat_<Vec3b> dstG = Mat_<Vec3b>(height, width, CV_8UC3);
		Mat_<Vec3b> dstR = Mat_<Vec3b>(height, width, CV_8UC3);

		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				Vec3b val = src.at<Vec3b>(i, j);
				dstB.at<Vec3b>(i, j)[0] = val[0];
				dstG.at<Vec3b>(i, j)[1] = val[1];
				dstR.at<Vec3b>(i, j)[2] = val[2];
				//dstR.at<Vec3b>(i, j) = (0, 0, val[2]);
			}
		}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);
		// Printing image:
		imshow("input image", src);
		imshow("Blue", dstB);
		imshow("Green", dstG);
		imshow("Red", dstR);
		// Saving image:
		waitKey();
	}

}
void colorToGrayscale() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		double t = (double)getTickCount(); // Get the current time [s]

		Mat_<Vec3b> src = imread(fname, 1);
		int height = src.rows;
		int width = src.cols;
		Mat_<Vec3b> dst = Mat_<Vec3b>(height, width, CV_8UC3);

		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				Vec3b val = src.at<Vec3b>(i, j);
				uchar avg = (val[0] + val[1] + val[2]) / 3;
				dst.at<Vec3b>(i, j) = Vec3b(avg, avg, avg);
			}
		}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);
		// Printing image:
		imshow("input image", src);
		imshow("Grayscale", dst);
		// Saving image:
		waitKey();
	}
}
void grayscaleToBlackAndWhite() {
	std::cout << "Threshold value(0<x<255)";
	int threshold;
	std::cin >> threshold;
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		double t = (double)getTickCount(); // Get the current time [s]

		Mat src = imread(fname, 0);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);

		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				uchar val = src.at<uchar>(i, j);
				if (val < threshold)
					dst.at<uchar>(i, j) = 0;
				else
					dst.at<uchar>(i, j) = 255;
			}
		}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);
		// Printing image:
		imshow("input image", src);
		imshow("Black and white", dst);
		// Saving image:
		waitKey();
	}
}
void rgbToHSV() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		double t = (double)getTickCount(); // Get the current time [s]

		Mat src = imread(fname, CV_LOAD_IMAGE_COLOR);
		int height = src.rows;
		int width = src.cols;
		Mat dstH = Mat(height, width, CV_8UC1);
		Mat dstS = Mat(height, width, CV_8UC1);
		Mat dstV = Mat(height, width, CV_8UC1);

		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				Vec3b val = src.at<Vec3b>(i, j);
				float r, g, b;
				//r = float(val[2] / 255);
				//g = float(val[1] / 255);
				//b = float(val[0] / 255);
				r = (float)val[2];
				b = (float)val[1];
				g = (float)val[0];
				float M = max(max(r,g), b);
				float m = min(min(r,g), b);
				float C = M - m;
				float H, S, V;
				V = M;
				if(V != 0)
					S = C / V;
				else // grayscale
					S = 0;
				if(C != 0) {
					if (M == r) H = 60 * (g - b) / C;
					if (M == g) H = 120 + 60 * (b - r) / C;
					if (M == b) H = 240 + 60 * (r - g) / C;
				}
				else // grayscale
					H = 0;
				if(H < 0)
					H = H + 360;

				uchar H_norm, S_norm, V_norm;
				H_norm = H * 255 / 360;
				S_norm = S * 255;
				V_norm = V * 255;

				dstH.at<uchar>(i, j) = 255-H_norm;
				dstS.at<uchar>(i, j) = S_norm;
				dstV.at<uchar>(i, j) = 255-V_norm;
			}
		}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);
		// Printing image:
		imshow("input image", src);
		imshow("Hue", dstH);
		imshow("Value", dstV);
		imshow("Saturation", dstS);
		// Saving image:
		waitKey();
	}
}
void copiedSelectedObjectInto(Mat src, Mat copiedObject) {
	int height = src.rows;
	int width = src.cols;
	copiedObject = Mat(height, width, CV_8UC1);
	// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
	// Varianta ineficienta (lenta)
	for (int i = 0; i<height; i++)
	{
		for (int j = 0; j<width; j++)
		{
			//if (sameColor(src, ));
		}
	}
}
bool isNotExternal(int i, int j, int width, int height) {
	if ( (i == 0) || (i == width) || (i == height) || (j == 0) || (j == width) || (j = height) )
		return false;
	return true;
}
void GeometricalFeaturesComputation(int event, int x, int y, int flags, void* param)
{
	//More examples: http://opencvexamples.blogspot.com/2014/01/detect-mouse-clicks-and-moves-on-image.html
	Mat* src = (Mat*)param;
	if (event == CV_EVENT_LBUTTONDBLCLK)
	{
		printf("Pos(x,y): %d,%d  Color(RGB): %d,%d,%d\n",
			x, y,
			(int)(*src).at<Vec3b>(y, x)[2],
			(int)(*src).at<Vec3b>(y, x)[1],
			(int)(*src).at<Vec3b>(y, x)[0]);

		int height = src->rows;
		int width = src->cols;
		Mat dst = Mat(height, width, CV_8UC1);

		Vec3b color = src->at<Vec3b>(y, x);
		int area = 0, nrRows = 0, nrCols = 0;
		int minimum_c = height;
		int maximum_c = 0;
		int minimum_r = width;
		int maximum_r = 0;

		float perimeter = 0.0;
		float center_of_mass_R = 0.0, center_of_mass_C = 0.;
		float angle_of_elongation = 0.0, thinness_ratio = 0.0, aspect_ratio = 0.0;

		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (src->at<Vec3b>(i, j) == color) {
					area++; // area
					nrCols = nrCols + j; // center of
					nrRows = nrRows + i;           // mass
					minimum_c = min(minimum_c, j);
					minimum_r = min(minimum_r, i);
					maximum_c = max(maximum_c, j);
					maximum_r = max(maximum_r, i);
				}
			}
		}
		center_of_mass_R = nrRows / area;
		center_of_mass_C = nrCols / area;
		// Angle of elongation
		float X1 = 0.0, X2 = 0.0, Y = 0.0;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (src->at<Vec3b>(i, j) == color) {
					Y += (i - center_of_mass_R)*(j - center_of_mass_C);
					X1 += (j - center_of_mass_C)*(j - center_of_mass_C);
					X2 += (i - center_of_mass_R)*(i - center_of_mass_R);
				}
			}
		}

		Y *= 2;
		Mat dst2 = src->clone();
		angle_of_elongation = 0.5*atan2(Y, X1 - X2);
		float ra = (float)center_of_mass_R + tan(angle_of_elongation) * (minimum_c - center_of_mass_C);
		float rb = (float)center_of_mass_R + tan(angle_of_elongation) * (maximum_c - center_of_mass_C);
		line(dst2, Point(minimum_c, ra), Point(maximum_c, rb), Scalar(0, 0, 0));
		imshow("bla", dst2);

		if (angle_of_elongation < 0)
			angle_of_elongation += CV_PI;
		angle_of_elongation *= 180 / CV_PI;
		// Perimeter
		int NP = 0, flag = 0;
		for (int i = 1; i < height - 1; i++)
		{
			for (int j = 1; j < width - 1; j++)
			{
				if (src->at<Vec3b>(i, j) == color) {
					if (flag == 0) {
						for (int rowz = i - 1; rowz <= i + 1; rowz++)
							for (int colz = j - 1; colz <= j + 1; colz++)
								if (src->at<Vec3b>(rowz, colz) != color)
									flag = 1;
					}
					if (flag)
						NP++;
					flag = 0;
				}
			}
		}
		perimeter = NP * CV_PI / 4;
		thinness_ratio = 4 * CV_PI * area / (perimeter*perimeter);
		std::cout << maximum_c << " " << minimum_c << " " << maximum_r << " " << minimum_r << std::endl;
		aspect_ratio = (float)((maximum_c - minimum_c) + 1) / ((maximum_r - minimum_r) + 1);
		std::cout << "Area: " << area << std::endl;
		std::cout << "Center of mass: " << center_of_mass_R << ", " << center_of_mass_C << std::endl;
		std::cout << "Angle of elongation axis: " << angle_of_elongation << std::endl;
		std::cout << "Perimeter: " << perimeter << std::endl;
		std::cout << "Thinness ratio: " << thinness_ratio << std::endl;
		std::cout << "Aspect ratio: " << aspect_ratio << std::endl;



	}
}
void geometricalFeatures() {
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_LOAD_IMAGE_COLOR);
		//Create a window
		namedWindow("My Window", 1);

		//set the callback function for any mouse event
		setMouseCallback("My Window", GeometricalFeaturesComputation, &src);

		//show the image
		imshow("My Window", src);

		// Wait until user press some key
		waitKey(0);
	}

}

bool sameColor(Mat img, int i1, int j1, int i2, int j2) {
	Vec3b colorOfPix1 = img.at<Vec3b>(i1, j1);
	Vec3b colorOfPix2 = img.at<Vec3b>(i2, j2);
	if( (colorOfPix1[0] == colorOfPix2[0]) && (colorOfPix1[1] == colorOfPix2[1]) && (colorOfPix1[2] == colorOfPix2[2]))
		return true;
	return false;
}


typedef struct objectPixel {
	int i;
	int j;
	int label;
};

void labelEquivalence() {
	
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat labels(height, width, CV_32SC1, Scalar(0));
		int label = 0;

		for (int i = 10; i<height-10; i++)
		{
			for (int j = 10; j<width-10; j++)
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
		Vec3b *colors = new Vec3b[label + 1];
		Mat *objects = new Mat[label];
		for (int k = 1; k <= label; k++) {
			colors[k] = Vec3b(d(gen), d(gen), d(gen));
			objects[k] = Mat(height, width, CV_32SC1);
		}
		std::cout << "Labels: " << label;
		Mat dst(height, width, CV_8UC3, Scalar(255, 255, 255));

		std::cout << "BLABLABLA";
		for (int i = 10; i<height - 10; i++)
		{
			for (int j = 10; j<width - 10; j++)
			{
				for (int k = 1; k <= label; k++) {
					if (labels.at<int>(i, j) == k) {
						dst.at<Vec3b>(i, j) = colors[k];
						objects[k].at<float>(i, j) = 255;
					}
				}
			}
		}
		for (int k = 1; k <= label; k++) {
			imshow("Object", objects[k]);
		}
		
		imshow("input image", src);
		imshow("colored labels", dst);
		delete [] colors;
		waitKey();
	}
}
int minL(std::vector<int> L) {
	int min = 999999;
	for(int y:L)
		if (y < min)
			min = y;
	return min;
}
void label2Equivalence() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat labels(height, width, CV_32SC1, Scalar(0));
		std::vector<std::vector<int>> edges;
		edges.resize(10000);
		int label = 0;
		//std::cout << "Aici 0.1" << std::endl;
		int di[4] = { -1, -1, -1, 0 };
		int dj[4] = { -1, 0, 1, -1 };
		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				if (src.at<uchar>(i, j) == 0 && labels.at<int>(i, j) == 0) {
					std::vector<int> L;
					for (int k = 0; k <= 3; k++) {
						//std::cout << "Aici.2" << std::endl;
						if (labels.at<int>(i + di[k], j + dj[k]) > 0)
							L.push_back(labels.at<int>(i + di[k], j + dj[k]));
					}
					//std::cout << "Aici.3" << std::endl;
					if (L.size() == 0) {
						label++;
						labels.at<int>(i, j) = label;
					}
					else
					{
						int x = minL(L);
						labels.at<int>(i, j) = x;
						//for  (int y = 0; y < L.size()-1; y++) {
						for (int y:L){	
						//	std::cout << "Aici.4" << std::endl;
							if (y != x) {
								edges[x].push_back(y);
								edges[y].push_back(x);
							}
						}
					}

				}
			}
		}
	//	std::cout << "Aici.5" << std::endl;
		int newLabel = 0;
		int *newLabels = new int[label + 1];
		//std::vector<int> newLabels;
		//newLabels.resize(2000);
	//	for (int y : newLabels)
		//	y = 0;
		for (int i = 1; i < label; i++) {
			if (newLabels[i] == 0)
			{
				newLabel++;
				std::queue<int> Q;
				newLabels[i] = newLabel;
				Q.push(i);
				while (!Q.empty()) {
					int x = Q.front();
					Q.pop();
					for (int y = 0; y <= edges[x].size(); y++) {
					//	std::cout << "Aici2" << std::endl;
						if (newLabels[y] == 0) {
							newLabels[y] = newLabel;
							Q.push(y);
						}
					}
				}

			}
		}
		//std::cout << "Aici3" << std::endl;
		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				labels.at<int>(i, j) = newLabels[labels.at<int>(i, j)];

			}
		}
		std::default_random_engine gen;
		std::uniform_int_distribution<int> d(0, 255);
		uchar x = d(gen);
		Vec3b *colors = new Vec3b[label + 1];
		for (int k = 1; k <= label; k++)
			colors[k] = Vec3b(d(gen), d(gen), d(gen));

		Mat dst(height, width, CV_8UC3, Scalar(255, 255, 255));
		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				for (int k = 0; k < label; k++)
					if (labels.at<int>(i, j) == k)
						dst.at<Vec3b>(i, j) = colors[k];

			}
		}


		std::cout << label << std::endl;
		std::cout << newLabel;
		
		imshow("input image", src);
		//imshow("output image", newLabels);
		imshow("output image2", dst);
		waitKey();
	}
}
void label22() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		int label = 0;
		Mat labels(height, width, CV_32SC1, Scalar(0));
		std::vector<std::vector<int>> edges;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (src.at<uchar>(i, j) == 0 && labels.at<int>(i, j) == 0) {
					std::vector<int> L;
					if (labels.at<int>(i, j - 1)) {
						if (labels.at<int>(i, j - 1)) {
							L.push_back(labels.at<int>(i, j - 1));
						}
					}else if (labels.at<int>(i - 1, j - 1)) {
						if (labels.at<int>(i - 1, j - 1)) {
							L.push_back(labels.at<int>(i - 1, j - 1));
						}
					}else if (labels.at<int>(i - 1, j)) {
						if (labels.at<int>(i - 1, j)) {
							L.push_back(labels.at<int>(i - 1, j));
						}
					}else if (labels.at<int>(i - 1, j + 1)) {
						if (labels.at<int>(i - 1, j + 1)) {
							L.push_back(labels.at<int>(i - 1, j + 1));
						}
					}
					if (L.size() == 0) {
						label++;
						edges.resize(label + 1);
						labels.at<int>(i, j) = label;
					}else {
						int x = minL(L);
						labels.at<int>(i, j) = x;
						for (int y : L)
							if (x != y) {
								edges[x].push_back(y);
								edges[y].push_back(x);
							}
					}


				}
			}
		}

		int newLabel = 0;
		std::vector<int> newLabels(label + 1);

		for (int i = 1; i <= label; i++) {
			if (newLabels.at(i) == 0) {
				newLabel++;
				std::queue<int> Q;
				newLabels.at(i) = newLabel;
				Q.push(int(i));
				while (!Q.empty()) {
					int x = Q.front();
					Q.pop();
					for (int y : edges[x])
						if (newLabels.at(y) == 0)
						{
							newLabels.at(y) = newLabel;
							Q.push(y);
						}
				}

			}

		}

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++) {
				labels.at<int>(i, j) = newLabels.at(labels.at<int>(i, j));
			}
		}
		std::default_random_engine gen;
		std::uniform_int_distribution<int> d(0, 255);
		uchar x = d(gen);
		Vec3b *colors = new Vec3b[label + 1];
		for (int k = 1; k <= label; k++)
			colors[k] = Vec3b(d(gen), d(gen), d(gen));

		Mat dst(height, width, CV_8UC3, Scalar(255, 255, 255));
		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				for (int k = 0; k < label; k++)
					if (labels.at<int>(i, j) == k)
						dst.at<Vec3b>(i, j) = colors[k];

			}
		}
		std::cout << label << std::endl;
		std::cout << newLabel;
		imshow("input image", src);
		imshow("output image", dst);


		waitKey();
	}
}

typedef struct borderPoint {
	int i;
	int j;
	int dir;
}borderP;

void borderTracing() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		double t = (double)getTickCount(); // Get the current time [s]

		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC3);
		std::vector<int> absoluteC, derivativeC;
		std::vector<borderP> borders;
		borderP P0, P1, Pn, Pn1;
		int offsetI[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };
		int offsetJ[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
		//borders.resize(10000);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (src.at<uchar>(i, j) == 0)
				{
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
		//	if (borders.size() == 2)
		//		P1 = borders.at(1);
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
			borders.push_back(borderP{ neighbors.at(currentPoint.dir).i , neighbors.at(currentPoint.dir).j , currentPoint.dir});

			std::cout << currentPoint.dir << " ";
			P1 = borders.at(1);
			Pn1 = borders.at(borders.size() - 2);
			Pn = borders.at(borders.size() - 1);
			//std::cout << "Size:" << borders.size();
		} while (!((borders.size() > 2) && ( Pn.i == P1.i && Pn.j == P1.j) && (Pn1.i == P0.i && Pn1.j == P0.j)));
		std::cout << std::endl;
		for (int i = 0; i < borders.size()-1; i++) {
			std::cout << (borders.at(i + 1).dir - borders.at(i).dir + 8) % 8 << " ";
		}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);
		// Printing image:
		imshow("input image", src);
		imshow("output image", dst);


		waitKey();
	}
}

void borderReconstruction() {
	int y0, x0, n;
	std::vector<int> ac;
	std::ifstream f;
	f.open("Images/Border/Reconstruct.txt");
	f >> y0 >> x0;
	f >> n;
	int dir;
	Mat src = imread("Images/Border/gray_background.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	int offsetI[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };
	int offsetJ[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	for (int i = 0; i < n; i++) {
		f >> dir;
		src.at<uchar>(y0 + offsetI[dir], x0 + offsetJ[dir]) = 0;
		y0 += offsetI[dir];
		x0 += offsetJ[dir];
	}
	imshow("Reconstruction", src);
	waitKey();
}

void dilationAction(int n, Mat src) {
	int offsetI[4] = { 0, -1, 1, 0 };
	int offsetJ[4] = { 1, 0, 0, -1 };
	Mat dst;
	if (n > 0)
	{
		src.copyTo(dst);
		for (int i = 1; i < src.rows - 1; i++)
			for (int j = 1; j < src.cols - 1; j++) {
				if (src.at<uchar>(i, j) == 0)
				{
					for (int k = 0; k < 3; k++) {
						dst.at<uchar>(i + offsetI[k], j + offsetJ[k]) = 0;
					}
				}
			}
		dilationAction(n-1, dst);
	}
	else {
		imshow("dst", src);
		waitKey(0);
	}
}

void dilation(int n) {
	Mat src;
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat dst;
		src.copyTo(dst);

		imshow("input image", src);

		dilationAction(n, dst);
		waitKey(0);
	}
}

void erosionAction(int n, Mat src) {
	int offsetI[4] = { 0, -1, 1, 0 };
	int offsetJ[4] = { 1, 0, 0, -1 };
	Mat dst;
	if (n > 0)
	{
		src.copyTo(dst);
		for (int i = 1; i < src.rows - 1; i++)
			for (int j = 1; j < src.cols - 1; j++) {
				if ((src.at<uchar>(i + 1, j) == 255 || src.at<uchar>(i + 1, j + 1) == 255 || src.at<uchar>(i + 1, j - 1) == 255 || src.at<uchar>(i - 1, j) == 255 || src.at<uchar>(i - 1, j + 1) == 255 || src.at<uchar>(i - 1, j - 1) == 255 || src.at<uchar>(i, j - 1) == 255 || src.at<uchar>(i, j + 1) == 255) && src.at<uchar>(i, j) == 0)
				{
					dst.at<uchar>(i, j) = 255;
				}
			}
		erosionAction(n-1, dst);
	}
	else {
		imshow("dst", src);
		waitKey(0);
	}
}

void erosion(int n) {
	Mat src;
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat dst;
		src.copyTo(dst);

		imshow("input image", src);

		erosionAction(n, dst);
		waitKey(0);
	}
}

void opening() {
	Mat src;
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat dst;
		src.copyTo(dst);
		int offsetI[4] = { 0, -1, 1, 0 };
		int offsetJ[4] = { 1, 0, 0, -1 };
			src.copyTo(dst);
			for (int i = 1; i < src.rows - 1; i++)
				for (int j = 1; j < src.cols - 1; j++) {
					if (src.at<uchar>(i, j) == 0)
					{
						for (int k = 0; k < 3; k++) {
							dst.at<uchar>(i + offsetI[k], j + offsetJ[k]) = 0;
						}
					}
				}
			Mat dst2;
			dst.copyTo(dst2);
			for (int i = 1; i < dst.rows - 1; i++)
				for (int j = 1; j < dst.cols - 1; j++) {
					if ((dst.at<uchar>(i + 1, j) == 255 || dst.at<uchar>(i + 1, j + 1) == 255 || dst.at<uchar>(i + 1, j - 1) == 255 || dst.at<uchar>(i - 1, j) == 255 || dst.at<uchar>(i - 1, j + 1) == 255 || dst.at<uchar>(i - 1, j - 1) == 255 || dst.at<uchar>(i, j - 1) == 255 || dst.at<uchar>(i, j + 1) == 255) && dst.at<uchar>(i, j) == 0)
					{
						dst2.at<uchar>(i, j) = 255;
					}
				}
			for (int i = 1; i < dst.rows - 1; i++)
				for (int j = 1; j < dst.cols - 1; j++) {
					if (dst.at<uchar>(i, j) == 0)
					{
						for (int k = 0; k < 3; k++) {
							dst2.at<uchar>(i + offsetI[k], j + offsetJ[k]) = 0;
						}
					}
				}
		imshow("input image", src);
		imshow("output image", dst2);

		waitKey(0);
	}
}

void closing() {
	Mat src;
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat dst;
		src.copyTo(dst);
		int offsetI[4] = { 0, -1, 1, 0 };
		int offsetJ[4] = { 1, 0, 0, -1 };
		src.copyTo(dst);
		for (int i = 1; i < src.rows - 1; i++)
			for (int j = 1; j < src.cols - 1; j++) {
				if ((src.at<uchar>(i + 1, j) == 255 || src.at<uchar>(i + 1, j + 1) == 255 || src.at<uchar>(i + 1, j - 1) == 255 || src.at<uchar>(i - 1, j) == 255 || src.at<uchar>(i - 1, j + 1) == 255 || src.at<uchar>(i - 1, j - 1) == 255 || src.at<uchar>(i, j - 1) == 255 || src.at<uchar>(i, j + 1) == 255) && src.at<uchar>(i, j) == 0)
				{
					dst.at<uchar>(i, j) = 255;
				}
			}
		Mat dst2;
		dst.copyTo(dst2);


		imshow("input image", src);
		imshow("output image", dst2);

		waitKey(0);
	}
}

void boundaryExtraction() {
	Mat src;
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, 0);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1, Scalar(255));
		Mat dst2 = Mat(height, width, CV_8UC1, Scalar(255));

		for (int i = 1; i <dst.rows - 1; i++)
			for (int j = 1; j < dst.cols - 1; j++) {
				if ((dst.at<uchar>(i + 1, j) == 255 || dst.at<uchar>(i + 1, j + 1) == 255 || dst.at<uchar>(i + 1, j - 1) == 255 || dst.at<uchar>(i - 1, j) == 255 || dst.at<uchar>(i - 1, j + 1) == 255 || dst.at<uchar>(i - 1, j - 1) == 255 || dst.at<uchar>(i, j - 1) == 255 || dst.at<uchar>(i, j + 1) == 255) && dst.at<uchar>(i, j) == 0)
				{
					dst2.at<uchar>(i, j) = 255;
				}
			}

		for (int i = 1; i < height; i++) {
			for (int j = 1; j < width; j++) {
				if (src.at<uchar>(i, j) == 0 && dst2.at<uchar>(i, j) != 0) {
					dst.at<uchar>(i, j) = 0;
				}
			}
		}

		imshow("Input", src);
		imshow("Output", dst);
		waitKey();
	}
}

void histogramMeanAndStdDev() {
	Mat src;
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1, Scalar(255));
		int h[256] = { 0 };

		float p[256] = { 0.0 };

		int M =height * width;
		float mean = 0.0;
		float standardDeviation = 0.0;

		for (int i = 1; i < height; i++) {
			for (int j = 1; j < width; j++) {
				h[src.at<uchar>(i, j)]++;
			}
		}

		showHistogram("Histogram", h, 256, 500);
		
		//printf("h(g): ");
		for (int i = 0; i < 256; i++) {
			p[i] = (float) h[i] / M;
			//printf("%d, ", h[i]);
		}
		/*
		printf("\n p(g): ");
			for (int i = 0; i < 256; i++) {
				printf("%f, ", p[i]);
			}
		showHistogram("Normalized Histogram", (int*) p, 256, 500);
		*/
		for (int i = 0; i < 256; i++) {
			mean += (float) i*p[i];
		}
		printf("\n Mean: %f", mean);

		for (int i = 0; i < 256; i++) {
			standardDeviation += (i - mean)*(i - mean)*p[i];
		}
		printf("\n Standard deviation: %f", sqrt(standardDeviation));
		imshow("Input", src);
	//	imshow("Output", dst);
		waitKey();
	}
}

void globalThresholding() {
	Mat src;
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1, Scalar(255));
		int minIntensity = 0;
		int maxIntensity = 255;
		int h[256] = { 0 };
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				h[src.at<uchar>(i, j)]++;
			}
		}/*
		for (int i = 0; i < 256; i++) {
			if (h[i] < minIntensity) {
				minIntensity = h[i];
			}
			if (h[i] > maxIntensity) {
				maxIntensity = h[i];
			}
		}*/
		showHistogram("Histogram", h, 256, 500);
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

		int treshold = (minIntensity + maxIntensity) / 2;

		float T = (minIntensity + maxIntensity)*1.0 / 2;
		float Tk1 = 0;
		do {
			Tk1 = T;
			int N1 = 0, N2 = 0, ug11 = 0, ug21 = 0;
			for (int i = minIntensity; i < T; i++)
				N1 += h[i];
			for (int i = T + 1; i <= maxIntensity; i++)
				N2 += h[i];

			for (int i = minIntensity; i < T; i++)
				ug11 += i*h[i];
			for (int i = T + 1; i <= maxIntensity; i++)
				ug21 += i*h[i];

			float ug1 = ug11*1.0 / N1;
			float ug2 = ug21*1.0 / N2;

			T = (ug1 + ug2)*1.0 / 2;
		} while (abs(T - Tk1) > 0.1);

		printf("\n Minimum intesity: %d, Maximum intensity: %d, T0: %f", minIntensity, maxIntensity, T);
	imshow("Input", src);
	//imshow("Output", dst);
	waitKey();
	}
}
void brightnessChange(int offset) {
	Mat src;
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1, Scalar(255));
		int h[256] = { 0 };
		int b[256] = { 0 };

		for (int i = 1; i < height; i++) {
			for (int j = 1; j < width; j++) {
				h[src.at<uchar>(i, j)]++;
			}
		}
		bool doNothing = false;
		showHistogram("Histogram", h, 256, 500);
		for (int i = 0; i < 256; i++) {
			if (i + offset < 0)
				doNothing = true;
			else if (i + offset > 255)
				doNothing = true;
			else 
				b[i + offset] = h[i];
		}
		showHistogram("Brightness Histogram", b, 256, 500);

		imshow("Input", src);
		//	imshow("Output", dst);
		waitKey();
	}
}

void contrastChange(int cmin, int cmax){
	Mat src;
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1, Scalar(255));
		int h[256] = { 0 };
		int c[256] = { 0 };
		int hmin = 99999, hmax = 0;
/*
		for (int i = 1; i < height; i++) {
			for (int j = 1; j < width; j++) {
				h[src.at<uchar>(i, j)]++;
			}
		}
		for (int i = 0; i < 256; i++) {
			hmin = min(hmin, h[i]);
			hmax = max(hmax, h[i]);
		}
		showHistogram("Histogram", h, 256, 500);
		for (int i = 0; i < 256; i++) {
				c[i] = cmin + (h[i] - hmin) * ((cmax - cmin) / (hmax - hmin));
		}
		showHistogram("Contrast Histogram", c, 256, 500);
*/
		int Imax = 0, Imin = 0;
		Imax = dst.at<uchar>(0, 0);
		Imin = dst.at<uchar>(0, 0);
		Mat gout(dst.rows, dst.cols, CV_8UC1);
		for (int i = 0; i < src.rows; i++)
			for (int j = 0; j < src.cols; j++) {
				if (Imax < dst.at<uchar>(i, j))
					Imax = dst.at<uchar>(i, j);
				if (Imin > dst.at<uchar>(i, j))
					Imin = dst.at<uchar>(i, j);
			}
		for (int i = 0; i < src.rows; i++)
			for (int j = 0; j < src.cols; j++)
				gout.at<uchar>(i, j) = cmin + (src.at<uchar>(i, j) - Imin)*((cmax - cmin)*1.0 / (Imax - Imin));

		imshow("Input", src);
		imshow("Output", gout);
		waitKey();
	}
}


int* computeHistogram(Mat* src) {
	int height = src->rows;
	int width = src->cols;
	int h[256] = { 0 };
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			h[src->at<uchar>(i, j)] += 1;
		}
	}
	return h;
}
void gammaCorrection() {
	char fname[MAX_PATH];
	Mat src;
	float gamma = 0.0;
	printf("Gamma: ");
	scanf("%f", &gamma);
	while (openFileDlg(fname))
	{
		src = imread(fname, 0);
		int height = src.rows;
		int width = src.cols;
		int L = 255;
		Mat dst = Mat(height, width, CV_8UC1);
		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				uchar gIn = src.at<uchar>(i, j);
				int gOut = (int)(L * pow((float)gIn / L, gamma));
				if (gOut < 0) {
					gOut = 0;
				}
				if (gOut > 255) {
					gOut = 255;
				}
				dst.at<uchar>(i, j) = gOut;
			}
		}
		imshow("input image", src);
		imshow("gamma correction", dst);
		int *h = computeHistogram(&src);
		int *ho = computeHistogram(&dst);
		showHistogram("Histogram input", h, 256, 500);
		showHistogram("Histogram output", ho, 256, 500);
		waitKey();
	}
}
void meanFilter3x3() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		Mat kernelK = Mat(3, 3, CV_8UC1);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i = 1; i < height-1; i++)
		{
			for (int j = 1; j < width-1; j++)
			{
				//*pana aici src *\\

				float S = 0;
				S = src.at<uchar>(i - 1, j - 1)		+ src.at<uchar>(i - 1, j)	    + src.at<uchar>(i - 1, j + 1) +
					src.at<uchar>(i, j - 1)			+ src.at<uchar>(i, j)			+ src.at<uchar>(i, j + 1) +
					src.at<uchar>(i + 1, j - 1)		+ src.at<uchar>(i + 1, j)		+ src.at<uchar>(i + 1, j + 1);
				dst.at<uchar>(i, j) = S / 9;

			}
		}
			// Printing image:
			imshow("input image", src);
			imshow("multiplicative image", dst);

			waitKey();
	}
}

void meanFilter5x5() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		Mat kernelK = Mat(5, 5, CV_8UC1);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i = 2; i < height - 2; i++)
		{
			for (int j = 2; j < width - 2; j++)
			{
				//*pana aici src *\\

				float S = 0;
				S =   src.at<uchar>(i-2, j-2)		+ src.at<uchar>(i-2, j-1)	 + src.at<uchar>(i-2, j)	 + src.at<uchar>(i-2, j+1)		+ src.at<uchar>(i-2, j+2)
					+ src.at<uchar>(i-1, j-2)	    + src.at<uchar>(i-1, j-1)	 + src.at<uchar>(i-1, j)	 + src.at<uchar>(i-1, j+1)		+ src.at<uchar>(i-1, j+2)
					+ src.at<uchar>(i, j-2)			+ src.at<uchar>(i, j-1)		 + src.at<uchar>(i, j)		 + src.at<uchar>(i, j+1)		+ src.at<uchar>(i, j+2)
					+ src.at<uchar>(i+1, j-2)		+ src.at<uchar>(i+1, j-1)	 + src.at<uchar>(i+1, j)	 + src.at<uchar>(i+1, j+1)		+ src.at<uchar>(i+1, j+2)
					+ src.at<uchar>(i+2, j-2)		+ src.at<uchar>(i+2, j-1)	 + src.at<uchar>(i+2, j)	 + src.at<uchar>(i+2, j+1)		+ src.at<uchar>(i+2, j+2);
				dst.at<uchar>(i, j) = S / 25;

			}
		}
		// Printing image:
		imshow("input image", src);
		imshow("multiplicative image", dst);

		waitKey();
	}
}

void gaussianFilter() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		Mat kernelK = Mat(3, 3, CV_8UC1);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i = 1; i < height - 1; i++)
		{
			for (int j = 1; j < width - 1; j++)
			{
				//*pana aici src *\\

				float S = 0;
				S = src.at<uchar>(i - 1, j - 1) + 2*src.at<uchar>(i - 1, j) + src.at<uchar>(i - 1, j + 1) +
					2*src.at<uchar>(i, j - 1)	+ 4*src.at<uchar>(i, j)		+ 2*src.at<uchar>(i, j + 1) +
					src.at<uchar>(i + 1, j - 1) + 2*src.at<uchar>(i + 1, j) + src.at<uchar>(i + 1, j + 1);
				dst.at<uchar>(i, j) = S / 16;

			}
		}
		// Printing image:
		imshow("input image", src);
		imshow("multiplicative image", dst);

		waitKey();
	}
}


void laplaceFilter() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		Mat kernelK = Mat(3, 3, CV_8UC1);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i = 1; i < height - 1; i++)
		{
			for (int j = 1; j < width - 1; j++)
			{
				//*pana aici src *\\

				float S = 0;
				S = 0*src.at<uchar>(i - 1, j - 1) + (-1) * src.at<uchar>(i - 1, j) + 0*src.at<uchar>(i - 1, j + 1) +
					(-1) * src.at<uchar>(i, j - 1) + 4 * src.at<uchar>(i, j) + (-1) * src.at<uchar>(i, j + 1) +
					0*src.at<uchar>(i + 1, j - 1) + (-1) * src.at<uchar>(i + 1, j) + 0*src.at<uchar>(i + 1, j + 1);
				if (S > 255.0)
					S = 255.0;
				if (S < 0.0)
					S = 0.0;
				dst.at<uchar>(i, j) = S;

			}
		}
		// Printing image:
		imshow("input image", src);
		imshow("multiplicative image", dst);

		waitKey();
	}
}
void highPassFilter() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		Mat kernelK = Mat(3, 3, CV_8UC1);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i = 1; i < height - 1; i++)
		{
			for (int j = 1; j < width - 1; j++)
			{
				//*pana aici src *\\

				float S = 0;
				S = 0*src.at<uchar>(i - 1, j - 1) + (-1) * src.at<uchar>(i - 1, j) + 0*src.at<uchar>(i - 1, j + 1) +
					(-1) * src.at<uchar>(i, j - 1) + 5 * src.at<uchar>(i, j) + (-1) * src.at<uchar>(i, j + 1) +
					0*src.at<uchar>(i + 1, j - 1) + (-1) * src.at<uchar>(i + 1, j) + 0*src.at<uchar>(i + 1, j + 1);
				if (S > 255.0)
					S = 255.0;
				if (S < 0.0)
					S = 0.0;
				dst.at<uchar>(i, j) = S;

			}
		}
		// Printing image:
		imshow("input image", src);
		imshow("multiplicative image", dst);

		waitKey();
	}
}

void centering_transform(Mat img) {
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; i < img.cols; i++) {
			img.at<float>(i, j) = ((i + j) & 1) ? -img.at<float>(i, j) : img.at<float>(i, j);
		}
	}
}
void magnitudeLogOfFourierCenteredSpectrum() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		
		Mat srcf;

		src.convertTo(srcf, CV_32FC1);

		centering_transform(srcf);

		Mat fourier;

		dft(srcf, fourier, DFT_COMPLEX_OUTPUT);
		Mat channels[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
		split(fourier, channels);

		Mat mag, phi;
		magnitude(channels[0], channels[1], mag);
		phase(channels[0], channels[1], phi);

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				mag.at<float>(i, j) = log(mag.at<float>(i, j) + 1);
			}
		}
		centering_transform(mag);
		normalize(mag, dst, 0, 255, NORM_MINMAX, CV_8UC1);

		imshow("input image", src);
		imshow("multiplicative image", dst);

		waitKey();
	}
}
void idealLowPass() {
	char fname[MAX_PATH];
	int radius = 0;
	printf("Radius: ");
	scanf("%d", &radius);
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, 0);
		Mat srcf;
		src.convertTo(srcf, CV_32FC1);
		centering_transform(srcf);
		Mat fourier;
		dft(srcf, fourier, DFT_COMPLEX_OUTPUT);
		Mat channels[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
		split(fourier, channels);
		Mat mag, phi;
		magnitude(channels[0], channels[1], mag);
		for (int i = 0; i < mag.rows; i++)
		{
			for (int j = 0; j < mag.cols; j++)
			{
				mag.at<float>(i, j) = log(mag.at<float>(i, j) + 1);
			}
		}
		int height = src.rows;
		int width = src.cols;
		for (int u = 0; u < height; u++)
		{
			for (int v = 0; v < width; v++)
			{
				int dist = (height / 2 - u)*(height / 2 - u) + (width / 2 - v)*(width / 2 - v);
				if (dist >(radius*radius)) {
					channels[0].at<float>(u, v) = 0;
					channels[1].at<float>(u, v) = 0;
				}
			}
		}
		Mat dst, dstf;
		merge(channels, 2, fourier);
		dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
		centering_transform(dstf);
		normalize(dstf, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		imshow("output", dst);
		waitKey();
	}
}

void idealHighPass() {
	int radius = 0;
	printf("Radius: ");
	scanf("%d", &radius);
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, 0);
		Mat srcf;
		src.convertTo(srcf, CV_32FC1);
		centering_transform(srcf);
		Mat fourier;
		dft(srcf, fourier, DFT_COMPLEX_OUTPUT);
		Mat channels[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
		split(fourier, channels);
		Mat mag, phi;
		magnitude(channels[0], channels[1], mag);
		for (int i = 0; i < mag.rows; i++)
		{
			for (int j = 0; j < mag.cols; j++)
			{
				mag.at<float>(i, j) = log(mag.at<float>(i, j) + 1);
			}
		}
		int height = src.rows;
		int width = src.cols;
		for (int u = 0; u < height; u++)
		{
			for (int v = 0; v < width; v++)
			{
				int dist = (height / 2 - u)*(height / 2 - u) + (width / 2 - v)*(width / 2 - v);
				if (dist <= (radius*radius)) {
					channels[0].at<float>(u, v) = 0;
					channels[1].at<float>(u, v) = 0;
				}
			}
		}
		Mat dst, dstf;
		merge(channels, 2, fourier);
		dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
		centering_transform(dstf);
		normalize(dstf, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		imshow("output", dst);
		waitKey();
	}
}



void gaussianCutLPF() {
	char fname[MAX_PATH];
	float a = 0;
	printf("Insert A: ");
	scanf("%f", &a);
	while (openFileDlg(fname))
	{

		Mat src = imread(fname, 0);
		Mat srcf;
		src.convertTo(srcf, CV_32FC1);
		centering_transform(srcf);
		Mat fourier;
		dft(srcf, fourier, DFT_COMPLEX_OUTPUT);
		Mat channels[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
		split(fourier, channels);
		Mat mag, phi;
		magnitude(channels[0], channels[1], mag);
		for (int i = 0; i < mag.rows; i++)
		{
			for (int j = 0; j < mag.cols; j++)
			{
				mag.at<float>(i, j) = log(mag.at<float>(i, j) + 1);
			}
		}
		int height = src.rows;
		int width = src.cols;
		for (int u = 0; u < height; u++)
		{
			for (int v = 0; v < width; v++)
			{
				float dist = (height / 2 - u)*(height / 2 - u) + (width / 2 - v)*(width / 2 - v);
				channels[0].at<float>(u, v) *= (exp(-dist / (a*a)));
				channels[1].at<float>(u, v) *= (exp(-dist / (a*a)));

			}
		}
		Mat dst, dstf;
		merge(channels, 2, fourier);
		dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
		centering_transform(dstf);
		normalize(dstf, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		imshow("output", dst);
		waitKey();
	}
}

void gaussianCutHPF() {
	char fname[MAX_PATH];
	float a = 0;
	printf("Insert A: ");
	scanf("%f", &a);
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, 0);
		Mat srcf;
		src.convertTo(srcf, CV_32FC1);
		centering_transform(srcf);
		Mat fourier;
		dft(srcf, fourier, DFT_COMPLEX_OUTPUT);
		Mat channels[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
		split(fourier, channels);
		Mat mag, phi;
		magnitude(channels[0], channels[1], mag);
		for (int i = 0; i < mag.rows; i++)
		{
			for (int j = 0; j < mag.cols; j++)
			{
				mag.at<float>(i, j) = log(mag.at<float>(i, j) + 1);
			}
		}
		int height = src.rows;
		int width = src.cols;
		for (int u = 0; u < height; u++)
		{
			for (int v = 0; v < width; v++)
			{
				float dist = (height / 2 - u)*(height / 2 - u) + (width / 2 - v)*(width / 2 - v);
				channels[0].at<float>(u, v) *= (1.0 - exp(-dist / (a*a)));
				channels[1].at<float>(u, v) *= (1.0 - exp (-dist / (a*a)));
			}
		}
		Mat dst, dstf;
		merge(channels, 2, fourier);
		dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
		centering_transform(dstf);
		normalize(dstf, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		imshow("output", dst);
		waitKey();
	}
}
Mat createConvolution(Mat *src, Mat *k, int sum) {
	int height = src->rows;
	int width = src->cols;
	Mat dst = Mat(height, width, CV_8UC1);
	int dim = k->rows;
	for (int i = dim / 2; i <= height - dim / 2 - 1; i++)
	{
		for (int j = dim / 2; j <= width - dim / 2 - 1; j++) {
			int val = 0;
			for (int ki = -dim / 2; ki <= dim / 2; ki++) {
				for (int m = -dim / 2; m <= dim / 2; m++) {
					val += (src->at<uchar>(i + ki, j + m) * k->at<int>(ki + dim / 2, m + dim / 2));
				}
			}
			if (sum == 1) {
				if (val < 0) {
					val = 0;
				}
				if (val > 255) {
					val = 255;
				}
			}

			dst.at<uchar>(i, j) = val / sum;
		}
	}
	return dst;
}

void cannyEdgeDetection() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat gradientX = Mat(height, width, CV_32FC1);
		Mat gradientY = Mat(height, width, CV_32FC1);
		Mat gradient = Mat(height, width, CV_32FC1);
		Mat direction = Mat::zeros(src.rows, src.cols, CV_32FC1);
		Mat suppression = Mat::zeros(src.rows, src.cols, CV_32FC1);
	
		for (int i = 1; i < height - 1; i++)
		{
			for (int j = 1; j < width - 1; j++)
			{
				//*pana aici src *\\

				float S = 0;
				S = (-1) * src.at<uchar>(i - 1, j - 1) + 0 * src.at<uchar>(i - 1, j) + 1 * src.at<uchar>(i - 1, j + 1) +
					(-2) * src.at<uchar>(i, j - 1) + 0 * src.at<uchar>(i, j) + 2 * src.at<uchar>(i, j + 1) +
					(-1) * src.at<uchar>(i + 1, j - 1) + 0 * src.at<uchar>(i + 1, j) + 1 * src.at<uchar>(i + 1, j + 1);
				gradientX.at<float>(i, j) = S;

			}
		}
		for (int i = 1; i < height - 1; i++)
		{
			for (int j = 1; j < width - 1; j++)
			{
				//*pana aici src *\\

				float S = 0;
				S = 1 * src.at<uchar>(i - 1, j - 1) + 2 * src.at<uchar>(i - 1, j) + 1 * src.at<uchar>(i - 1, j + 1) +
					0 * src.at<uchar>(i, j - 1) + 0 * src.at<uchar>(i, j) + 0 * src.at<uchar>(i, j + 1) +
					(-1) * src.at<uchar>(i + 1, j - 1) + (-2) * src.at<uchar>(i + 1, j) + (-1) * src.at<uchar>(i + 1, j + 1);
				gradientY.at<float>(i, j) = S;

			}
		}
		for (int i = 1; i < height-1; i++)
		{
			for (int j = 1; j < width-1; j++)
			{
				gradient.at<float>(i, j) = sqrt(gradientX.at<float>(i, j) * gradientX.at<float>(i, j) + (gradientY.at<float>(i, j) * gradientY.at<float>(i, j))) / (4*sqrt(2));
				direction.at<float>(i, j) = (float)atan2(gradientY.at<float>(i, j), gradientX.at<float>(i, j));
			}
		}


		for (int i = 1; i < src.rows - 1; i++)
		{
			for (int j = 1; j < src.cols - 1; j++)
			{
				if ((direction.at<float>(i, j) <= PI / 8 && direction.at<float>(i, j) >= -PI / 8) || (direction.at<float>(i, j) >= 7 * PI / 8) || (direction.at<float>(i, j)<-7 * PI / 8))
				{
					if (gradient.at<float>(i, j) >= gradient.at<float>(i, j - 1) && gradient.at<float>(i, j) >= gradient.at<float>(i, j + 1))
						suppression.at<float>(i, j) = gradient.at<float>(i, j);
					else
						suppression.at<float>(i, j) = 0;
				}

				else if ((direction.at<float>(i, j)<3 * PI / 8 && direction.at<float>(i, j)>PI / 8) || (direction.at<float>(i, j) <= -5 * PI / 8 && direction.at<float>(i, j) > -7 * PI / 8))
				{
					if (gradient.at<float>(i, j) >= gradient.at<float>(i - 1, j + 1) && gradient.at<float>(i, j) >= gradient.at<float>(i + 1, j - 1))
						suppression.at<float>(i, j) = gradient.at<float>(i, j);
					else
						suppression.at<float>(i, j) = 0;
				}
				else if ((direction.at<float>(i, j) >= 3 * PI / 8 && direction.at<float>(i, j) < 5 * PI / 8) || (direction.at<float>(i, j) <= -3 * PI / 8 && direction.at<float>(i, j) > -5 * PI / 8))
				{
					if (gradient.at<float>(i, j) >= gradient.at<float>(i - 1, j) && gradient.at<float>(i, j) >= gradient.at<float>(i + 1, j))
						suppression.at<float>(i, j) = gradient.at<float>(i, j);
					else
						suppression.at<float>(i, j) = 0;
				}
				else
				{
					if (gradient.at<float>(i, j) >= gradient.at<float>(i - 1, j - 1) && gradient.at<float>(i, j) >= gradient.at<float>(i + 1, j + 1))
						suppression.at<float>(i, j) = gradient.at<float>(i, j);
					else
						suppression.at<float>(i, j) = 0;
				}



			}
		}

		//imshow("result", suppression);
		Mat ceva;
			suppression.convertTo(ceva, CV_8UC1);
			imshow("Suppression", ceva);
		Mat showGrad;
		gradient.convertTo(showGrad, CV_8UC1);


		int h[255] = { 0 };
		for (int i = 1; i < gradient.rows - 1; i++) {
			for (int j = 1; j < gradient.cols - 1; j++) {
				h[showGrad.at<uchar>(i, j)]++;
			}
		}

		float noNonEdge = 0.9f * (height * width - (float)h[0]);
		float noEdge = 0.1 * ((height - 2) * (width - 2) - (float)h[0]);
		printf("%f\n", noNonEdge);

		int s = 0;
		int thresh = 0;
		for (int i = 1; i < 255; i++) {
			s += h[i];
			thresh = i;
			if (s > noNonEdge)
				break;
		}

		int th = thresh;
		float tl = 0.4 * th;

		printf("%d %d\n", tl, th);

		Mat res = Mat(height, width, CV_32FC1);

		for (int i = 1; i < gradient.rows - 1; i++) {
			for (int j = 1; j < gradient.cols - 1; j++) {
				if (gradient.at<float>(i, j) >= th) {
					res.at<float>(i, j) = 255;
				}
				if (gradient.at<float>(i, j) <= th && gradient.at<float>(i, j) > tl) {
					res.at<float>(i, j) = 128;
				}
				if (gradient.at<float>(i, j) <= tl) {
					res.at<float>(i, j) = 0;
				}
			}
		}
		Mat res2 = Mat::zeros(res.rows, res.cols, CV_32FC1);

		for (int i = 1; i < res.rows - 1; i++) {
			for (int j = 1; j < res.cols - 1; j++) {
				if (res.at<float>(i, j) == 255 && res2.at<float>(i, j) == 0) {

					res2.at<float>(i, j) = 255;
					std::queue < Point2i > Q;
					Q.push(Point2i(i, j));

					while (!Q.empty()) {

						Point2i q = Q.front();
						Q.pop();

						int di[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
						int dj[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

						for (int k = 0; k < 8; k++) {
							if (q.x + di[k] < 0 || q.y + dj[k] < 0 || q.x + di[k] > res.rows - 1 || q.y + dj[k] > res.cols - 1) {
								continue;
							}
							if ((res.at<float>(q.x + di[k], q.y + dj[k]) == 255 && res2.at<float>(q.x + di[k], q.y + dj[k]) == 0) || (res.at<float>(q.x + di[k], q.y + dj[k]) == 128 && res2.at<float>(q.x + di[k], q.y + dj[k]) == 0)) {
								res2.at<float>(q.x + di[k], q.y + dj[k]) = 255;
								Q.push(Point2i(q.x + di[k], q.y + dj[k]));
							}
						}
					}
				}
			}
		}

		for (int i = 1; i < res2.rows - 1; i++) {
			for (int j = 1; j < res2.cols - 1; j++) {
				if (res2.at<float>(i, j) == 128) {
					res2.at<float>(i, j) = 0;
				}
			}
		}

		Mat resShow;
		res.convertTo(resShow,CV_8UC1);
		Mat resShow2;
		res2.convertTo(resShow2, CV_8UC1);
		imshow("final", resShow2);
		imshow("adaptive", resShow);
		imshow("input", src);
		imshow("magnitude", showGrad);
		waitKey();
	}
}
void matritaCod() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		double t = (double)getTickCount(); // Get the current time [s]

		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		Mat kernelK = Mat(3, 3, CV_8UC1);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				//*pana aici src *\\

				float S = 0;
				S = src.at<uchar>(i - 1, j - 1) + src.at<uchar>(i - 1, j) + src.at<uchar>(i - 1, j + 1) +
					src.at<uchar>(i, j - 1) + src.at<uchar>(i, j) + src.at<uchar>(i, j + 1) +
					src.at<uchar>(i + 1, j - 1) + src.at<uchar>(i + 1, j) + src.at<uchar>(i + 1, j + 1);
				dst.at<uchar>(i, j) = S / 9;

				//*pana aici src *\\
							}
			}

			// Get the current time again and compute the time difference [s]
			t = ((double)getTickCount() - t) / getTickFrequency();
			// Print (in the console window) the processing time in [ms] 
			printf("Time = %.3f [ms]\n", t * 1000);
			// Printing image:
			imshow("input image", src);
			imshow("multiplicative image", dst);

			waitKey();
		}
	}
}
int main()
{
	int op;
	do
	{
		system("cls");
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 1 - Open image\n");
		printf(" 2 - Open BMP images from folder\n");
		printf(" 3 - Image negative - diblook style\n");
		printf(" 4 - BGR->HSV\n");
		printf(" 5 - Resize image\n");
		printf(" 6 - Canny edge detection\n");
		printf(" 7 - Edges in a video sequence\n");
		printf(" 8 - Snap frame from live video\n");
		printf(" 9 - Mouse callback demo\n");
		printf(" 10 - [L1] Additive factor (Grayscale)\n");
		printf(" 11 - [L1] Multiplicative factor (Grayscale) + Save image\n");
		printf(" 12 - [L1] 4 squares(Color 256x256)\n");
		printf(" 13 - [L1] Matrix inverse\n");
		printf(" 14 - [L2] R, G, B channels\n");
		printf(" 15 - [L2] Conversion color - grayscale\n");
		printf(" 16 - [L2] Conversion grayscale - black and white\n");
		printf(" 17 - [L2] RGB to HSV\n");

		printf(" 18 - [L4] Geometrical features \n");
		//printf(" 19 - [L4] Projection \n");
		/*L5*/
		printf(" 20 - [L5] Labeling (equivalence classes)\n");
		printf(" 21 - [L5] Labeling2 (equivalence classes)\n");
		/*L6*/
		printf(" 22 - [L6] Border tracing algorithm\n");
		printf(" 23 - [L6] Reconstruction \n");
		/*L7*/
		printf(" 24 - [L7] Dilation\n");
		printf(" 25 - [L7] Erosion \n");
		printf(" 26 - [LL] Opening\n");
		printf(" 27 - [L7] Closing\n");
		printf(" 28 - [L7] Boundary extraction\n");
		printf(" 29 - [L7] Region filling -- Nu!\n");
		/*L8*/	
		printf(" 30 - [L8] Histogram mean & standard deviation\n");
		printf(" 31 - [L8] Basic global tresholding algorithm\n");
		printf(" 32 - [L8] Brightness change\n");
		printf(" 33 - [L8] Contrast change\n");
		printf(" 34 - [L8] Gamma correction\n");
		printf(" 35 - [L8] Histogram equalization -- Nu!\n"); 
		/*L9*/
		printf(" 36 - [L9] Mean filter (3x3)\n");
		printf(" 37 - [L9] Mean filter (5x5)\n");
		printf(" 38 - [L9] Gaussian filter\n");
		printf(" 39 - [L9] Laplace filter\n");
		printf(" 40 - [L9] High-pass filte\n");
		printf(" 41 - [L9] Magnitude log of the centered Fourier spectrum\n");
		printf(" 42 - [L9] Ideal low-pass filter  \2\n");
		printf(" 43 - [L9] Ideal high-pass filter  \2\n");
		printf(" 44 - [L9] Gaussian-cut LPF ?\n ");
		printf(" 45 - [L9] Gaussian-cut HPF ?\n");
		/*L11*/
		printf("46 - [L11] Canny edge detection\n");

		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d",&op);
		int n;
		switch (op)
		{
			case 1:
				testOpenImage();
				break;
			case 2:
				testOpenImagesFld();
				break;
			case 3:
				testParcurgereSimplaDiblookStyle(); //diblook style
				break;
			case 4:
				//testColor2Gray();
				testBGR2HSV();
				break;
			case 5:
				testResize();
				break;
			case 6:
				testCanny();
				break;
			case 7:
				testVideoSequence();
				break;
			case 8:
				testSnap();
				break;
			case 9:
				testMouseClick();
				break;
			case 10:
				int incVal;
				printf("Additive factor:\n");
				scanf("%d", &incVal);
				additiveGrayscale(incVal);
				break;
			case 11:
				float mulVal;
				printf("Multiplicative factor:\n");
				scanf("%f", &mulVal);
				multiplicativeGrayscale(mulVal);
				break;
			case 12:
				coloredSquares();
				break;
			case 13:
				inverseMatrix();
				break;
			case 14:
				rgbChannels();
				break;
			case 15:
				colorToGrayscale();
				break;
			case 16:
				grayscaleToBlackAndWhite();
				break;
			case 17:
				rgbToHSV();
				break;
			case 18:
				geometricalFeatures();
				break;
			case 19:
				//projections();
				break;
			case 20:
				labelEquivalence();
				break;
			case 21:
				//label2Equivalence();
				label22();
				break;
			case 22:
				borderTracing();
				break;
			case 23:
				borderReconstruction();
				break;
			case 24:
				std::cout << "How many times?" << std::endl;
				std::cin >> n;
				dilation(n);
				break;
			case 25:
				std::cout << "How many times?" << std::endl;
				std::cin >> n;
				erosion(n);
				break;
			case 26:
				opening();
				break;
			case 27:
				closing();
				break;
			case 28:
				boundaryExtraction();
				break;
			case 29:
				break;
				printf(" 30 - [L8] Histogram mean & standard deviation\n");
				printf(" 31 - [L8] Basic global tresholding algorithm\n");
				printf(" 32 - [L8] Brightness change\n");
				printf(" 33 - [L8] Contrast change\n");
				printf(" 34 - [L8] Gamma correction\n");
				printf(" 35 - [L8] Histogram equalization\n");
			case 30:
				histogramMeanAndStdDev();
				break;
			case 31:
				globalThresholding();
				break;
			case 32:
				std::cout << "Brightness: " << std::endl;
				std::cin >> n;
				brightnessChange(n);
				break;
			case 33:
				std::cout << "Cmin"<< std::endl;
				std::cin >> n;
				int m;
				std::cout << "Cmax" << std::endl;
				std::cin >> m;
				contrastChange(n,m);
				break;
			case 34:
				//std::cout << "Gamma: " << std::endl;
				//std::cin >> n;
				gammaCorrection();
				break;
			case 35:
				
				break;
			case 36:
				meanFilter3x3();
				break;
			case 37:
				meanFilter5x5();
				break;
			case 38:
				gaussianFilter();
				break;
			case 39:
				laplaceFilter();
				break;
			case 40:
				highPassFilter();
				break;
			case 41:
				magnitudeLogOfFourierCenteredSpectrum();
				break;
			case 42:
				idealLowPass();
				break;
			case 43:
				idealHighPass();
				break;
			case 44:
				gaussianCutLPF();
				break;
			case 45:
				gaussianCutHPF();
				break;
			case 46: 
				cannyEdgeDetection();
			
		}
	}
	while (op!=0);
	return 0;
}