#include <iostream> 
#include <string>   
#include <filesystem>
#include <opencv2/core/core.hpp>       
#include <opencv2/highgui/highgui.hpp> 
#include <SFML/Graphics.hpp>
namespace fs = std::filesystem;

const std::string pathing = "../frame-final-video";

cv::Size getFrameSize(){
    sf::Image frame;
    frame.loadFromFile(pathing+"/0.jpg");
    sf::Vector2 frameSize = frame.getSize();
    return cv::Size(frameSize.x,frameSize.y);
}

void writeFrames(cv::VideoWriter& outputVideo){
    int frameTotal = std::distance(fs::directory_iterator(pathing), fs::directory_iterator{}); 
    for(int frameNumber; frameNumber<frameTotal; frameNumber++){
        outputVideo.write(cv::imread(pathing + '/' + std::to_string(frameNumber) + ".jpg"));
    }
}

void frameToVideo(std::string name, int FPS){
    cv::Size frameSize = getFrameSize();
    cv::VideoWriter outputVideo("ASCII_"+name, cv::VideoWriter::fourcc('m','p','4','v'), FPS, frameSize, true);
    
    writeFrames(outputVideo);
}