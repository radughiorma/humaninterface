//============================================================================
// Name        : HelloWorld.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <sstream>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;

//This function threshold the HSV image and create a binary image
IplImage* GetThresholdedImage(IplImage* imgHSV){
       IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);

       cvInRangeS(imgHSV, cvScalar(170,160,60), cvScalar(180,256,256), imgThresh);
       return imgThresh;
}

IplImage* imgTracking;
CvSize imgFrameSize;
int lastX = -1;
int lastY = -1;

const int maxX = 1680;
const int maxY = 1050;
double xScreenScale = -1;
double yScreenScale = -1;

void mouse(){
//	CGWarpMouseCursorPosition
}

void trackFace(IplImage* frame, double_t scale = 1.3){
//Load the haar cascade //haarcascade_frontalface_alt
	//haarcascade_mcs_eyepair_small
	// IMAGE PREPARATION: //
	IplImage* gray = cvCreateImage( cvSize(frame->width,frame->height), 8, 1 );
	IplImage* small_img = cvCreateImage(cvSize( cvRound(frame->width/scale), cvRound(frame->height/scale)), 8, 1);
	cvCvtColor( frame, gray, CV_BGR2GRAY );
	cvResize( gray, small_img, CV_INTER_LINEAR );
	cvEqualizeHist( small_img, small_img );

	CvHaarClassifierCascade* hc =
	cvLoadHaarClassifierCascade("/Users/radug/github/local/opencv-2.4.2/data/haarcascades/haarcascade_frontalface_alt.xml", imgFrameSize);
//Detect face in image
	CvSeq* face = cvHaarDetectObjects(small_img, hc, cvCreateMemStorage(), scale,2, CV_HAAR_DO_CANNY_PRUNING,cvSize(100,100), cvSize(200,200));

	for(int i = 0; i < (face ? face->total : 0); i++ ) {
		CvRect* r = (CvRect*)cvGetSeqElem( face, i );
		cvRectangle(frame,  cvPoint(r->x*scale,r->y*scale), cvPoint((r->x+r->width)*scale,(r->y+r->height)*scale), cvScalar(0,255,0), 5);
	};
}

void trackObject(IplImage* imgThresh){

    // Calculate the moments of 'imgThresh'
	CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
	    cvMoments(imgThresh, moments, 1);
    double moment10 = cvGetSpatialMoment(moments, 1, 0);
    double moment01 = cvGetSpatialMoment(moments, 0, 1);
    double area = cvGetCentralMoment(moments, 0, 0);

     // if the area<1000, I consider that the there are no object in the image and it's because of the noise, the area is not zero
    if(area>1000){
        // calculate the position of the ball
        int posX = moment10/area;
        int posY = moment01/area;

       if(lastX>=0 && lastY>=0 && posX>=0 && posY>=0)
        {

    	   // Draw a yellow line from the previous point to the current point
           cvLine(imgTracking, cvPoint(imgFrameSize.width - posX, posY), cvPoint(imgFrameSize.width - lastX, lastY), cvScalar(0,255,0), 4);

//           cvLine(imgTracking, cvPoint(-posX, posY), cvPoint(-lastX, lastY), cvScalar(0,0,255), 4);

//           cvLine(imgTracking, cvPoint(lastX, lastY), cvPoint(posX, posY), cvScalar(0,0,255), 4);
        }

//       char str[200];

       std::ostringstream stringStream;
		int posX1 = maxX - posX * xScreenScale;
		int posY1 = posY * yScreenScale;
		stringStream << "/Users/radug/Documents/workspace/OpenCV/./click -x " << posX1 << " -y " << posY1;
         std::string copyOfStr = stringStream.str();
//         cout<< copyOfStr << endl;
//         cout << stringStream;

         system( copyOfStr.data());
         lastX = posX;
        lastY = posY;
    }

     free(moments);
}

int main() {

	CvCapture * capture ;
	capture = cvCreateCameraCapture ( CV_CAP_ANY ) ;

	if  ( ! capture )  {

	   cout << "Opening video stream imposible"<< "\n" <<endl;
	   return  1 ;

	}



//	cvNamedWindow ( "Display Image" , CV_WINDOW_AUTOSIZE ) ;
//	cvNamedWindow ("Red Object", CV_WINDOW_AUTOSIZE);


	IplImage * frame =0;
	 frame = cvQueryFrame(capture);
	      if(!frame) return -1;
    //create a blank image and assigned to 'imgTracking' which has the same size of original video
	imgFrameSize = cvGetSize(frame);
    imgTracking=cvCreateImage(imgFrameSize,IPL_DEPTH_8U, 3);
    cvZero(imgTracking); //covert the image, 'imgTracking' to black

    xScreenScale = maxX/imgFrameSize.width;
    yScreenScale = maxY/imgFrameSize.height;

    cout << "xScreenScale:"<< xScreenScale << "imgXFrameSize: " << imgFrameSize.width << "\n" <<endl;
    cout << "yScreenScale:"<< xScreenScale << "imgYFrameSize: " << imgFrameSize.height << "\n" <<endl;
//  cvNamedWindow("Video");
//  cvNamedWindow("Ball");

	char key = '1';
	while (key != 'q' && key != 'Q'){
		 frame = cvQueryFrame ( capture ) ;

		 if(!frame) break;

		 frame = cvCloneImage(frame);

		 cvSmooth(frame, frame, CV_GAUSSIAN,3,3); //smooth the original image using Gaussian kernel

		 IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);
		 cvCvtColor(frame, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV
		 IplImage* imgThresh = GetThresholdedImage(imgHSV);

		 cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN,3,3); //smooth the binary image using Gaussian kernel

		 //track the possition of the ball
		 trackObject(imgThresh);
		 //track face
//		 trackFace(frame, 1.5);

		 // Add the tracking image and the frame
//		            cvAdd(frame, imgTracking, frame);
//		            std::stringstream sstr;
//		            sstr << lastX << ":"<< lastY;
//		            cout<<sstr;

//		             cvShowImage("Ball", imgThresh);
//		             cvShowImage("Video", frame);
		             cvShowImage("Video", imgTracking);

		             //Clean up used images
		             cvReleaseImage(&imgHSV);
		             cvReleaseImage(&imgThresh);
		             cvReleaseImage(&frame);
	//	picture = cvRetrieveFrame ( capture ) ;
	//	cv::Mat image = imread("/Users/radug/Documents/workspace/OpenCV/workspaces/1stWorkspace/HelloWorld/src/boldt.jpg", IMWRITE_JPEG_QUALITY);

//		 imshow( "Display Image", image );
//		 cvShowImage( "Display Image", image );

		 key = cvWaitKey(1);

	}

	cvReleaseCapture ( & capture ) ;
	cvDestroyWindow ( "Video" ) ;
//	cout << "!!!Hello World!!!" +mat.cols << endl; // prints !!!Hello World!!!
	return 0;
}
