#include <iostream> 
#include <string>   
#include <filesystem>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <SFML/Graphics.hpp>
namespace fs = std::filesystem;

const std::string pathing = "frame-final-video";

std::string getVideoName(const std::string pathVideo){
    std::string videoName;
    unsigned int index = 0;
    do {
        videoName = fs::path(pathVideo).parent_path().string();
        if(index!=0){
            videoName += std::to_string(index);
        }
        index++;
    }
    while(fs::exists("videoASCII/"+videoName+".mp4"));
    return videoName;
}

cv::Size getFrameSize(){
    sf::Image frame;
    frame.loadFromFile(pathing+"/0.jpg");
    sf::Vector2 frameSize = frame.getSize();
    return cv::Size(frameSize.x,frameSize.y);
}

void writeFrames(cv::VideoWriter& outputVideo){
    int frameTotal = std::distance(fs::directory_iterator(pathing), fs::directory_iterator{});
    for(int frameNumber=0; frameNumber<frameTotal; frameNumber++){
        outputVideo.write(cv::imread(pathing + '/' + std::to_string(frameNumber) + ".jpg"));
    }
}

void frameToVideo(const std::string pathVideo, const int FPS){
    cv::Size frameSize = getFrameSize();
    cv::VideoWriter outputVideo("videoASCII/"+getVideoName(pathVideo)+".mp4", cv::VideoWriter::fourcc('m','p','4','v'), FPS, frameSize, true);
    writeFrames(outputVideo);
}