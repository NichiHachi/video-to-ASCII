#include <iostream> 
#include <string>   
#include <filesystem>
#include <opencv2/core/core.hpp>       
#include <opencv2/highgui/highgui.hpp> 
#include <SFML/Graphics.hpp>

#include "loading_bar.hpp"

namespace fs = std::filesystem;

const std::string pathing = "frame_final_product/";

int frame_to_video(std::string name, int FPS){
    sf::Image image;
    image.loadFromFile("frame_final_product/0.jpg");
    sf::Vector2 dim_image = image.getSize();

    cv::VideoWriter outputVideo ("ASCII_"+name,cv::VideoWriter::fourcc('m','p','4','v'),FPS,cv::Size(dim_image.x,dim_image.y),true);

    std::cout << "Writing in progress..." << std::endl;

    int percent=-1;
    int total_frame = std::distance(fs::directory_iterator("frame_final_product"), fs::directory_iterator{}); 
    int frame_number=0;
    for(const auto & entry : fs::directory_iterator(pathing)){
        outputVideo.write(cv::imread(pathing+std::to_string(frame_number)+".jpg"));
        frame_number++;
        if(percent != 100*frame_number/total_frame){
            percent = 100*frame_number/total_frame;
            loading_bar(percent);
        }
    }
    std::cout << std::endl;
    return 1;
}