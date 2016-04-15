

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <cstring>
#include <iostream>
#include <opencv2/videoio.hpp>

bool cut = true;
void my_Exit();
cv::Mat createOne(std::vector<cv::Mat> & images, int cols, int min_gap_size);
double BM_length(char* pass);

int main(int argc, char** argv)
{
	char* pass_filename = argv[1];
	
	//std::cout << pass_filename << std::endl;

	if (argc < 2) pass_filename = "test.mp4";


	CvCapture* capture = cvCaptureFromFile(pass_filename);
	double my_length = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 352);
	cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 288);

	std::string my_filename = "test";

	/*double length = BM_length(pass_filename); //another way to check length

	printf("Your video length is ");
	printf("%d", int(length));
	printf(" frames \n");
	*/
	printf("Your video length is ");
	printf("%d", int(my_length));
	printf(" frames \n");


	// Get one frame
	IplImage* frame = nullptr;

	uint my_frame = int(my_length) / 16;  //capture every my_frame frames

	printf("Images will be cut every ");
	printf("%d", my_frame);
	printf(" frames \n");

	std::vector<cv::Mat> test_framelist; // container for all captured frames

										 
	if (cut)
	{

		for (uint j = 1; j <= 16; j++)
		{
			for (uint i = 1; i < my_frame; i++)
			{

				frame = cvQueryFrame(capture);
			}

			std::stringstream frameNum; //generating name for saved images
			frameNum << j;
			if (j <= 9)
				my_filename += "0";
			my_filename += frameNum.str();
			my_filename += ".jpg";

			test_framelist.push_back(cv::cvarrToMat(frame, true));
			printf("Image # ");
			printf("%d", j);
			printf(" captured ");
			//IplImage* RGB_frame = frame;
			//cvCvtColor(frame,RGB_frame,CV_YCrCb2BGR);
			//cvWaitKey(1000);
			cvSaveImage(my_filename.c_str(), frame);
			//cvSaveImage("cam.jpg" ,RGB_frame);
			my_filename = "test";
			printf(" and Image # ");
			printf("%d", j);
			printf(" saved \n");
		}
	}
	//cvWaitKey(10);

	// Release the capture device housekeeping
	cvReleaseCapture(&capture);
	//cvDestroyWindow( "mywindow" );

	cv::imwrite("yourcollage.jpg", createOne(test_framelist, 4, 1));

	my_Exit();

	return 0;
}




double BM_length(char* pass) //another method to check length
{
	cv::VideoCapture myvid;
	int count = 0;
	//myvid.open("test2.avi");
	myvid.open(pass);

	if (!myvid.isOpened()) {
		printf("Capture not open \n");
	}



	double x = myvid.get(CV_CAP_PROP_FRAME_COUNT);

	return x;
}


//while (True) :
//	# Capture frame - by - frame
//	ret, frame = video_capture.read()
//	if not ret:
//break
//
//count += 1


void my_Exit()
{
	using namespace std;
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
	cout << "Press Enter key to close the programm ..." << endl;
	cin.get();
}


cv::Mat createOne(std::vector<cv::Mat> & images, int cols, int min_gap_size) //thanks to Guanta on opencv.org
{
	// finding max dimensions
	int max_width = 0;
	int max_height = 0;
	for (int i = 0; i < images.size(); i++) {
		// type check
		// helped in some cases, where captured images were empty
		if (i > 0 && images[i].type() != images[i - 1].type()) {
			std::cerr << "WARNING:createOne failed, different types of images";
			return cv::Mat();
		}
		max_height = std::max(max_height, images[i].rows);
		max_width = std::max(max_width, images[i].cols);
	}
	// number of images in y direction
	int rows = std::ceil(images.size() / cols);

	// create our result-matrix
	cv::Mat result = cv::Mat::zeros(rows*max_height + (rows - 1)*min_gap_size,
		cols*max_width + (cols - 1)*min_gap_size, images[0].type());

	size_t i = 0;
	int current_height = 0;
	int current_width = 0;
	for (int y = 0; y < rows; y++) {
		for (int x = 0; x < cols; x++) {
			if (i >= images.size()) // just a check
				return result;
			// get the ROI in our result-image
			cv::Mat to(result,
				cv::Range(current_height, current_height + images[i].rows),
				cv::Range(current_width, current_width + images[i].cols));
			// copy the current image to the ROI
			images[i++].copyTo(to);
			current_width += max_width + min_gap_size;
		}
		// next line - reset width and update height
		current_width = 0;
		current_height += max_height + min_gap_size;
	}
	return result;
}
