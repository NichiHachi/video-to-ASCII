#include "loading_bar.hpp"
#include "chara_to_matrix.hpp"
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
        _mkdir("frame_ascii_color_1");
        _mkdir("frame_ascii_color_2");
        _mkdir("frame_final_product");
        _mkdir("frame_video");
    #elif __linux__
        mkdir("frame_ascii");
        mkdir("frame_ascii_color_1");
        mkdir("frame_ascii_color_2");
        mkdir("frame_final_product");
        mkdir("frame_video");
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
    //ASKING ALL VALUES TO THE USER:
    // - video_path: The name of the video
    // - FPS_ASCII_Video: The number of FPS of the ASCII video
    // - multi_color_selection: How many color the ascii need to be 
    // - color_selection: The color of the font and background
    // - precision_selection: How accurate is the ASCII

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


    int multi_color_selection;
    do{
        cout << "Multi-Color" << endl;
        cout << "1) Bicolor: Black and White" << endl;
        cout << "2) Tricolor: Black, White and Grey" << endl;
        cout << "Enter a selection: ";
        cin >> multi_color_selection;
        if(multi_color_selection!=1 && multi_color_selection!=2){
            cout << "Selection incorrect, please select a value between 1 and 2." << endl << endl;
        }
    }while(multi_color_selection!=1 && multi_color_selection!=2);
    cout << endl;


    bool isTricolor;
    int color_selection;
    if(multi_color_selection==1){
        isTricolor = false;
        do{
            cout << "Color of the video" << endl;
            cout << "1) White background with Black font" << endl;
            cout << "2) Black background with White font" << endl;
            cout << "Enter a selection: ";
            cin >> color_selection;
            if(color_selection!=1 && color_selection!=2){
                cout << "Selection incorrect, please select a value between 1 and 2." << endl << endl;
            }
        }while(color_selection!=1 && color_selection!=2);
        cout << endl;
    }
    else{
        isTricolor = true;
        do{
            cout << "Color of the video" << endl;
            cout << "1) White background with Black and Grey font" << endl;
            cout << "2) Black background with White and Grey font" << endl;
            cout << "3) Grey background with Black and White font" << endl;
            cout << "Enter a selection: ";
            cin >> color_selection;
            if(color_selection!=1 && color_selection!=2 && color_selection!=3){
                cout << "Selection incorrect, please select a value between 1 and 3." << endl << endl;
            }
        }while(color_selection!=1 && color_selection!=2 && color_selection!=3);
        cout << endl;
    }


    int precision_selection;
    do{
        cout << "Degree of accuracy" << endl;
        cout << "1) Normal" << endl << "2) High" << endl << "3) Very High" << endl;
        cout << "Enter a selection: ";
        cin >> precision_selection;
        if(precision_selection<1 || precision_selection>3){
                cout << "Selection incorrect, please select a value between 1 and 3." << endl << endl;
        }
    }while(precision_selection<1 || precision_selection>3);
    cout << endl;

    //END ASKING VALUES TO THE USER
    //MAKING THE VIDEO :

    if(precision_selection==3){
        chara_to_matrix("characters_low");
    }
    else{
        chara_to_matrix("characters");
    }


    video_to_frame(video_path,FPS_Original_Video,FPS_ASCII_Video);
    cout << endl;


    //Just to see how long it takes to make the ascii frames (Which is a LOT :c)
    time_t time_before_making_ascii_frame = time(NULL);
    cout <<"Transforming Frames into ASCII..." << endl;
    cout << "Grab a cup of coffee and lay down, this operation takes some time !" << endl;
    frame_to_ascii(precision_selection,color_selection,isTricolor);
    cout <<"Time spent making the ASCII frames: " << time(NULL)-time_before_making_ascii_frame << " seconds" << endl;
    cout << endl;

    
    cout <<  "Transforming ASCII into Frames..." << endl;
    if(!isTricolor){
        ascii_to_frame(color_selection);
    }
    else{
        ascii_to_frame_tricolor(color_selection);
    }
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