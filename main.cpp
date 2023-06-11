#include "loading_bar.hpp"
#include "chara_to_matrice.hpp"
#include "video_to_frame.hpp"
#include "frame_to_ascii.hpp"
#include "ascii_to_frame.hpp"
#include "frame_to_video.hpp"
#include <filesystem>
#include <ctime>
#include <direct.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>       
#include <opencv2/highgui/highgui.hpp> 
using namespace std;

int main(int argv, char** args){
    create_directories("frame_ascii");
    create_directories("frame_final_product");
    create_directories("frame_video");
    //Make the list of character matrices
    if(!chara_to_matrice()){
        cout << "The transformation of characters into a matrice has failled...";
        return 0;
    }

    //Remove any file in Frame Video
    for(const auto & entry : directory_iterator("frame_video")){
        remove(entry);
    }

    
    string video_path;
    bool valid_name = false;

    while(!valid_name){
        cout <<"Enter the video name with the format (.mp4 .avi ...): ";
        cin >> video_path;

        cv::VideoCapture videoCapture(video_path);
        if(videoCapture.isOpened()){
            valid_name = true;
        }
        else{
            cout << "Impossible to open the video. Try Again." << endl;
        }
        videoCapture.release();
    }
    
    cv::VideoCapture Original_Video(video_path);
    double FPS_Original_Video = Original_Video.get(cv::CAP_PROP_FPS);
    Original_Video.release();

    int FPS_ASCII_Video=0;
    while(FPS_ASCII_Video<=0 || FPS_ASCII_Video>FPS_Original_Video){
        cout << "Enter the number of FPS wanted: ";
        cin >> FPS_ASCII_Video;
        if(FPS_ASCII_Video<=0){
            cout << "The number of FPS cannot be negative or equal to 0." << endl;
        }
        else if(FPS_ASCII_Video>FPS_Original_Video){
            cout << "The number of FPS cannot exceed the FPS of the original video (" << FPS_Original_Video << ")." << endl;
        }
    }

    video_to_frame(video_path,FPS_Original_Video,FPS_ASCII_Video);
    cout << endl;


    time_t time_before_making_ascii_frame = time(NULL);
    vector<vector<string>> list_matrice_characters = txt_to_list();

    //Remove any file in Frame ASCII
    for(const auto & entry : directory_iterator("frame_ascii")){
        remove(entry);
    }

    int precision;
    cout << "Degree of accuracy" << endl;
    cout << "1) Low" << endl << "2) Medium (Recommended)" << endl << "3) High" << endl;
    cout << "Enter a selection: ";
    cin >> precision;
    if(precision<1){
        precision = 1;
    }
    else if(precision > 2){
        precision = 4;
    }

    cout << endl << "Transforming Frames into ASCII..." << endl;
    cout << "Grab a cup of coffee and lay down, this operation takes some time !" << endl;

    int total_frame = distance(directory_iterator("frame_video"), directory_iterator{});
    int frame_number = 0;

    for(const auto & entry : directory_iterator("frame_video")){
        ascii_transform(std::to_string(frame_number)+".jpg",list_matrice_characters,precision);
        frame_number++;
        loading_bar(total_frame,frame_number);
    }
    cout << endl;
    cout <<"Time spent making the ASCII frames : " << time(NULL)-time_before_making_ascii_frame << " seconds" << endl;
    cout << endl;


    //Remove any file in Frame Final Product
    for(const auto & entry : directory_iterator("frame_final_product")){
        remove(entry);
    }

    //Make the ascii frame
    int selection;
    cout << "Video Quality" << endl;
    cout << "1) Low" << endl << "2) Medium (Recommended)" << endl << "3) High" << endl;
    cout << "Enter a selection: ";
    cin >> selection;
    int font_size;

    switch(selection){
        case(1):
            font_size = 10;
            break;
        case(2):
            font_size = 20;
            break;
        default:
            font_size = 30;
            break;
    }
    cout << endl;
    ascii_to_frame(font_size);
    cout << endl;

    //Encoding the Video
    frame_to_video(video_path,FPS_ASCII_Video);
    cout << endl;

    cout << "Press Enter to End...";
    cin.ignore();

    return 1;
}