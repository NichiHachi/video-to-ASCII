#include "chara_to_matrix.h"
#include "video_to_frame.h"
#include "frame_to_ascii.h"
#include "ascii_to_frame.h"
#include "frame_to_video.h"

#include <filesystem>
#include <ctime>
#include <cstdio>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>

#ifdef _WIN32
    #include <direct.h>
#elif __linux__
    #include <sys/stat.h>
#endif

namespace fs = std::filesystem;

int main(int argv, char** args){
    #ifdef _WIN32
        _mkdir("frame_ascii");
        _mkdir("frame_ascii_color_1");
        _mkdir("frame_ascii_color_2");
        _mkdir("frame_final_product");
        _mkdir("frame_video");
    #elif __linux__
        mkdir("frame_ascii",0777);
        mkdir("frame_ascii_color_1",0777);
        mkdir("frame_ascii_color_2",0777);
        mkdir("frame_final_product",0777);
        mkdir("frame_video",0777);
    #endif
    
    for(const auto & entry : fs::directory_iterator("frame_video")){
        fs::remove(entry);
    }

    for(const auto & entry : fs::directory_iterator("frame_ascii_color_1")){
        fs::remove(entry);
    }
    
    for(const auto & entry : fs::directory_iterator("frame_ascii_color_2")){
        fs::remove(entry);
    }
    
    for(const auto & entry : fs::directory_iterator("frame_ascii")){
        fs::remove(entry);
    }

    for(const auto & entry : fs::directory_iterator("frame_final_product")){
        fs::remove(entry);
    }

    std::string video_path;
    int FPS_ASCII_Video;
    bool isTricolor;
    int color_sub_selection;
    int accuracy_selection;
    int FPS_Original_Video;

    if(argv==1){
        //ASKING ALL VALUES TO THE USER:
        // - video_path: The name of the video
        // - FPS_ASCII_Video: The number of FPS of the ASCII video
        // - color_selection: How many color the ascii need to be 
        // - color_sub_selection: The color of the font and background
        // - accuracy_selection: How accurate is the ASCII

        bool valid_name;
        do{
            std::cout <<"Enter the video name with the format (.mp4 .avi ...): ";
            std::cin >> video_path;

            cv::VideoCapture videoCapture(video_path);
            if(videoCapture.isOpened()){
                valid_name = true;
            }
            else{
                std::cout << "Impossible to open the video. Try Again." << std::endl;
                valid_name = false;
            }
            videoCapture.release();
        }while(!valid_name);
        std::cout << std::endl;

        cv::VideoCapture Original_Video(video_path);
        int FPS_Original_Video = Original_Video.get(cv::CAP_PROP_FPS);
        Original_Video.release();

        do{
            std::cout << "Enter the number of FPS wanted: ";
            std::cin >> FPS_ASCII_Video;
            if(FPS_ASCII_Video<=0){
                std::cout << "The number of FPS cannot be negative or equal to 0." << std::endl;
            }
            else if(FPS_ASCII_Video>FPS_Original_Video){
                std::cout << "The number of FPS cannot exceed the FPS of the original video (" << FPS_Original_Video << ")." << std::endl;
            }
        }while((FPS_ASCII_Video<=0 || FPS_ASCII_Video>FPS_Original_Video));
        std::cout << std::endl;


        int color_selection;
        do{
            std::cout << "Multi-Color" << std::endl;
            std::cout << "1) Bicolor: Black and White" << std::endl;
            std::cout << "2) Tricolor: Black, White and Grey" << std::endl;
            std::cout << "Enter a selection: ";
            std::cin >> color_selection;
            if(color_selection!=1 && color_selection!=2){
                std::cout << "Selection incorrect, please select a value between 1 and 2." << std::endl << std::endl;
            }
        }while(color_selection!=1 && color_selection!=2);
        std::cout << std::endl;


        bool isTricolor;
        if(color_selection==1){
            isTricolor = false;
            do{
                std::cout << "Color of the video" << std::endl;
                std::cout << "1) White background with Black font" << std::endl;
                std::cout << "2) Black background with White font" << std::endl;
                std::cout << "Enter a selection: ";
                std::cin >> color_sub_selection;
                if(color_sub_selection!=1 && color_sub_selection!=2){
                    std::cout << "Selection incorrect, please select a value between 1 and 2." << std::endl << std::endl;
                }
            }while(color_sub_selection!=1 && color_sub_selection!=2);
            std::cout << std::endl;
        }
        else{
            isTricolor = true;
            do{
                std::cout << "Color of the video" << std::endl;
                std::cout << "1) White background with Black and Grey font" << std::endl;
                std::cout << "2) Black background with White and Grey font" << std::endl;
                std::cout << "3) Grey background with Black and White font" << std::endl;
                std::cout << "Enter a selection: ";
                std::cin >> color_sub_selection;
                if(color_sub_selection!=1 && color_sub_selection!=2 && color_sub_selection!=3){
                    std::cout << "Selection incorrect, please select a value between 1 and 3." << std::endl << std::endl;
                }
            }while(color_sub_selection!=1 && color_sub_selection!=2 && color_sub_selection!=3);
            std::cout << std::endl;
        }

        do{
            std::cout << "Degree of accuracy" << std::endl;
            std::cout << "1) Normal" << std::endl << "2) High" << std::endl << "3) Very High" << std::endl;
            std::cout << "Enter a selection: ";
            std::cin >> accuracy_selection;
            if(accuracy_selection<1 || accuracy_selection>3){
                    std::cout << "Selection incorrect, please select a value between 1 and 3." << std::endl << std::endl;
            }
        }while(accuracy_selection<1 || accuracy_selection>3);
        std::cout << std::endl;
        //END ASKING VALUES TO THE USER
    }
    else{
        video_path = args[1];
        FPS_ASCII_Video = atoi(args[2]);
        isTricolor = atoi(args[3])-1;
        color_sub_selection = atoi(args[4]);
        accuracy_selection = atoi(args[5]);

        std::ifstream file("FPS.txt");
        std::string number;
        std::getline(file,number);
        FPS_Original_Video = std::stoi(number);
        file.close();
    }

    //MAKING THE VIDEO
    if(accuracy_selection==3){
        chara_to_matrix("characters_low");
    }
    else{
        chara_to_matrix("characters");
    }

    video_to_frame(video_path,FPS_Original_Video,FPS_ASCII_Video);

    frame_to_ascii(accuracy_selection,color_sub_selection,isTricolor);
    
    if(!isTricolor){
        ascii_to_frame_bicolor(color_sub_selection);
    }
    else{
        ascii_to_frame_tricolor(color_sub_selection);
    }

    frame_to_video(video_path,FPS_ASCII_Video);

    return 0;
}