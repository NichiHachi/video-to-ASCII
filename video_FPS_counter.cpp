#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>

int main(int argc, char *argv[]) {
    if(argc==1){
        return 1;
    }
    
    std::string videoFilePath = argv[1];
    cv::VideoCapture cap(videoFilePath);

    if (!cap.isOpened()) {
        std::ofstream file("FPS.txt");
        file << "-1";
        file.close();
        return 1;
    }

    int fps = cap.get(cv::CAP_PROP_FPS);

    std::ofstream file("FPS.txt");
    file << fps; 
    file.close();

    return 0;
}