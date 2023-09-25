#include <iostream>
#include <opencv2/opencv.hpp>

int video_to_frame(std::string video_path, int frame_video, int frame_ascii){ 
    cv::VideoCapture videoCapture(video_path);
    int frameCount = 0;
    int frame_number;
    int how_many_frame_to_skip = frame_video/frame_ascii;

    std::cout << "0 frames created.";
    cv::Mat frame;
    while (videoCapture.read(frame)) {
        if(frameCount%(how_many_frame_to_skip)==0){
            frame_number = frameCount/how_many_frame_to_skip;
            std::string frameFileName = "frame_video/"+std::to_string(frame_number) + ".jpg";
            cv::imwrite(frameFileName, frame);
            if(frame_number%25==0){
                std::cout << "\r" << frame_number <<" frames extracted.";
            }
        }
        frameCount++;
    }
    
    std::cout << "\r" << frame_number+1 <<" frames extracted."<< std::endl;
    videoCapture.release();

    return 1;
}