#include "loading_bar.hpp"
#include "chara_to_matrice.hpp"
#include "video_to_frame.hpp"
#include "frame_to_ascii.hpp"
#include "ascii_to_frame.hpp"
#include "frame_to_video.hpp"

#include <filesystem>
#include <ctime>
#include <cstdio>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>       
#include <opencv2/highgui/highgui.hpp> 

#ifdef _WIN32
    #include <direct.h>
#elif __linux__
    #include <sys/stat.h>
#endif

using namespace std;
using namespace cv;
namespace fs = std::filesystem;


int main(int argv, char** args){    
    #ifdef _WIN32
        _mkdir("frame_ascii");
        _mkdir("frame_final_product");
        _mkdir("frame_video");
    #elif __linux__
        mkdir("frame_ascii");
        mkdir("frame_final_product");
        mkdir("frame_video");
    #endif

    //Remove any files in Frame Video
    for(const auto & entry : fs::directory_iterator("frame_video")){
        fs::remove(entry);
    }
    
    //Remove any files in Frame ASCII
    for(const auto & entry : fs::directory_iterator("frame_ascii")){
        fs::remove(entry);
    }

    //Remove any files in Frame Final Product
    for(const auto & entry : fs::directory_iterator("frame_final_product")){
        fs::remove(entry);
    }
    
    //ASKING ALL VALUES TO THE USER:
    // - video_path: The name of the video
    // - FPS_ASCII_Video: The number of FPS of the ASCII video
    // - color_selection: The color of the font and background
    // - precision: How accurate is the ASCII
    // - quality_selection: How big the resolution of the frames is

    bool valid_name;

    string video_path;
    do{
        cout <<"Enter the video name with the format (.mp4 .avi ...): ";
        cin >> video_path;

        VideoCapture videoCapture(video_path);
        if(videoCapture.isOpened()){
            valid_name = true;
        }
        else{
            cout << "Impossible to open the video. Try Again." << endl;
            valid_name = false;
        }
        videoCapture.release();
    }while(!valid_name);
    cout << endl;
    

    VideoCapture Original_Video(video_path);
    double FPS_Original_Video = Original_Video.get(CAP_PROP_FPS);
    Original_Video.release();

    int FPS_ASCII_Video;
    do{
        cout << "Enter the number of FPS wanted: ";
        cin >> FPS_ASCII_Video;
        if(FPS_ASCII_Video<=0){
            cout << "The number of FPS cannot be negative or equal to 0." << endl;
        }
        else if(FPS_ASCII_Video>FPS_Original_Video){
            cout << "The number of FPS cannot exceed the FPS of the original video (" << FPS_Original_Video << ")." << endl;
        }
    }while((FPS_ASCII_Video<=0 || FPS_ASCII_Video>FPS_Original_Video));
    cout << endl;


    int color_selection;
    do{
        cout << "Color of the video" << endl;
        cout << "1) White background and Black font" << endl;
        cout << "2) Black background and White font" << endl;
        cout << "Enter a selection: ";
        cin >> color_selection;
        if(color_selection!=1 && color_selection!=2){
            cout << "Selection incorrect, please select a value between 1 and 2." << endl << endl;
        }
    }while(color_selection!=1 && color_selection!=2);
    cout << endl;


    int precision;
    do{
        cout << "Degree of accuracy" << endl;
        cout << "1) Low" << endl << "2) Medium" << endl << "3) High (Recommended for low quality video)" << endl;
        cout << "Enter a selection: ";
        cin >> precision;
        if(precision<1 || precision>3){
                cout << "Selection incorrect, please select a value between 1 and 3." << endl << endl;
        }
    }while(precision<1 || precision>3);
    cout << endl;


    int quality_selection;
    do{
        cout << "Video Quality" << endl;
        cout << "1) Low" << endl << "2) Medium (Recommended)" << endl << "3) High" << endl;
        cout << "Enter a selection: ";
        cin >> quality_selection;
        if(quality_selection<1 || quality_selection>3){
                cout << "Selection incorrect, please select a value between 1 and 3." << endl << endl;
        }
    }while(quality_selection<1 || quality_selection>3);
    cout << endl;

    //END ASKING VALUES TO THE USER
    //MAKING THE VIDEO :

    //Make the list of character matrices (The txt file)
    if(!chara_to_matrice()){
        cout << "The transformation of characters into a matrice has failled...";
        return 0;
    }


    video_to_frame(video_path,FPS_Original_Video,FPS_ASCII_Video);
    cout << endl;


    //Just to see how long it takes to make the ascii frames (Which is a LOT :c)
    time_t time_before_making_ascii_frame = time(NULL);
    cout <<"Transforming Frames into ASCII..." << endl;
    cout << "Grab a cup of coffee and lay down, this operation takes some time !" << endl;
    frame_to_ascii(precision,color_selection,false);
    cout <<"Time spent making the ASCII frames : " << time(NULL)-time_before_making_ascii_frame << " seconds" << endl;
    cout << endl;

    
    ascii_to_frame(quality_selection,color_selection);
    cout << endl;
    cout << endl;


    //Encoding the Video
    frame_to_video(video_path,FPS_ASCII_Video);
    cout << endl;

    
    ios_base::sync_with_stdio(false);
    cout << "Press Enter to End...";
    cin.ignore();

    return 1;
}