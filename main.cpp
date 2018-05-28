#include <chrono>
#include <ctime>
#include <iomanip>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "stdio.h"
int main(int argc, char *argv[])
{
    cv::VideoCapture cap(0);
    //缩放至640*480
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    if(!cap.isOpened())
    {
        return -1;
    }
    cv::Mat frame;
    cv::Mat edges;

    bool stop = false;
    char buf[100];
    while(!stop)
    {
        cap>>frame;

        //高斯变换
//        cv::cvtColor(frame, edges, CV_BGR2GRAY);
//        cv::GaussianBlur(edges, edges, cv::Size(7,7), 1.5, 1.5);
//        cv::Canny(edges, edges, 0, 30, 3);
        cv::cvtColor(frame, edges, CV_BGR2BGRA);

        //旋转180度
        cv::Mat matRotation = cv::getRotationMatrix2D( cv::Point(edges.cols / 2, edges.rows / 2), 180, 1 );
        cv::Mat matRotatedFrame;
        cv::warpAffine( edges, matRotatedFrame, matRotation, edges.size() );

        {
            auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            auto time = std::localtime(&t);
            strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", time);
            //添加时间
            cv::putText(matRotatedFrame, buf, cv::Point(10,50),CV_FONT_HERSHEY_COMPLEX,0.5, cv::Scalar(0,0,0));
        }
        cv::imshow( "当前视频", matRotatedFrame );

        //cv::imshow("当前视频",edges);
        if(cv::waitKey(30) >=0)
            stop = true;
    }
    return 0;
}
