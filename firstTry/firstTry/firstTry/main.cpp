#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;


string type2str(int type);

//Mat change_rgb(Mat *& resource);

void nayatani(Mat & src);
int clamp(int f, int min, int max);


int main(int argc, char** argv)
{	
	cout << "Have " << argc << " arguments:" << endl;
	for (int i = 0; i < argc; ++i) {
	std::cout << argv[i] << std::endl;
	}
	
	//IplImage* image = cvLoadImage("C:\\Users\\latree\\Dropbox\\CS510-Computational_Photography\\project\\firstTry\\firstTry\\apple.png");
	//Mat img = Mat(image);
	Mat img = imread(argv[1]);
	Mat luv_img;
	Mat gamma_mapping_img;

	cvtColor(img, luv_img, CV_BGR2Luv);


	gamma_mapping_img = luv_img;

	nayatani(gamma_mapping_img);

	cvtColor(gamma_mapping_img, gamma_mapping_img, CV_BGR2GRAY);
	
	imshow("Display window: ", gamma_mapping_img);
	waitKey(0);
	
	
	
	//cout << img.at<Vec3b>(0,0) << endl;
	
	/*
	// this does work
	bgrPixel[2] = bgrPixel[2] + 1;
	*/

	/*
	cout << "RGB: " <<img.at<Vec3b>(0, 0) << endl;

	//  display RGB value by converting vec3b to point3
	Point3i luv = Point3i(img.at<Vec3b>(0,0));
	cout << "Point3 type: " << luv << endl;
	cout << "L is : " << luv.x << endl;
	cout << "U is : " << luv.y << endl;
	cout << "v is : " << luv.z << endl;
	*/




	//point3_ to Vec
	//operator Vec<_Tp, 3>() const;
	
	//cout << img.type() << endl;

	//string ty = type2str(img.type());

	//cout << "Type = " << ty << endl;
	
	//namedWindow("Display window", WINDOW_AUTOSIZE);// Create a window for display.
	//imshow("Display window", img);                   // Show our image inside it.

	system("pause");
	return 0;
	
}


string type2str(int type) {
	string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {

	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}


Mat change_rgb(Mat resource){
	for (int x = 0; x > resource.rows; x++)
	{
		for (int y = 0; y > resource.cols; y++){

			Vec3b color = resource.at<Vec3b>(Point(x, y));
			if (color[0] > 10 && color[1] > 10 && color[2]> 10)
			{
				color[0] = 0;
				color[1] = 0;
				color[2] = 0;

				resource.at<Vec3b>(Point(x, y)) = color;
			}
		}

	}
	return resource;
}

int clamp(int f, int min, int max){
	return (f) < (min) ? (min) : (f) > (max) ? (max) : (f);
}


void nayatani(Mat & src){
	double hue, qhue, kbr, suv, gamma;
	double adaptlum = 20.0;
	double u_white = 0.20917;
	double v_white = 0.48810;

	for (int i = 0; i < src.rows; i++){
		for (int j = 0; j < src.cols; j++){
			src.at<Vec3b>(i, j)[1] = (int) (src.at<Vec3b>(i, j)[1] - u_white);
			src.at<Vec3b>(i, j)[2] = (int)(src.at<Vec3b>(i, j)[2] - v_white);
			Vec3b luv = src.at<Vec3b>(i, j);


			hue = atan2(luv[2], luv[1]);
			qhue = -0.01585 - 0.03016*cos(hue) - 0.04556*cos(2 * hue) - 0.02667*cos(3 * hue) - 0.00295*cos(4 * hue) + 0.14592*sin(hue) + 0.05084*sin(2 * hue) - 0.01900*sin(3 * hue) - 0.00764*sin(4 * hue);
			kbr = 0.2717*(6.469 + 6.362*pow(adaptlum, 0.4495)) / (6.469 + pow(adaptlum, 0.4495));

			suv = 13 * pow(pow(luv[1], 2) + pow(luv[2], 2), 0.5);
			gamma = 1 + (-0.1340*qhue + 0.0872*kbr)*suv;
			
			src.at<Vec3b>(i, j)[0] = clamp((int)(gamma*src.at<Vec3b>(i, j)[0] * 2.5599), 0, 255);
		}
	}

}

/*
#include <opencv\cv.h>
#include <opencv\highgui.h>

using namespace cv;
int main(){


Mat image;

VideoCapture cap;
cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
cap.open(0);

namedWindow("window", 1);
while (1)
{

try
{
cap >> image;

imshow("window", image);
}
catch (Exception& e)
{
const char* err_msg = e.what();
std::cout << "exception caught: imshow:\n" << err_msg << std::endl;
}
waitKey(33);
}

}

*/



/*
#include <opencv\cv.h>
#include <opencv\highgui.h>

using namespace cv;

int main(int argc, char** argv)
{


IplImage* img = cvLoadImage("C:\\Users\\latree\\Documents\\Visual Studio 2013\\Projects\\FirstOpenCV\\FirstOpenCV\\opencv-logo.jpg"); //change the name (image.jpg) according to your Image filename.
cvNamedWindow("Example1", CV_WINDOW_NORMAL);
cvShowImage("Example1", img);
cvWaitKey(0);
cvReleaseImage(&img);
cvDestroyWindow("Example1");
return 0;
}
*/