#include <string>
#include <opencv2/opencv.hpp>

int videoToFrame(std::string videoPath, int frame_video, int frame_ascii){ 
    cv::VideoCapture videoCapture(videoPath);
    int frameCount = 0;
    int frameNumber;
    int how_many_frame_to_skip = frame_video/frame_ascii;

    cv::Mat frame;
    while (videoCapture.read(frame)) {
        if(frameCount%(how_many_frame_to_skip)==0){
            frameNumber = frameCount/how_many_frame_to_skip;
            std::string frameFileName = "frame-video/"+std::to_string(frameNumber) + ".jpg";
            cv::imwrite(frameFileName, frame);
        }
        frameCount++;
    }
    
    videoCapture.release();

    return 1;
}