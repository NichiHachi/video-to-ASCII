#include <iostream> 
#include <string>   
#include <filesystem>
#include <opencv2/core/core.hpp>       
#include <opencv2/highgui/highgui.hpp> 
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;
namespace fs = filesystem;
using namespace cv;

const string pathing = "frame_final_product/";

int frame_to_video(string name, int FPS){
    Image image;
    image.loadFromFile("frame_final_product/0.jpg");
    Vector2 dim_image = image.getSize();

    VideoWriter outputVideo ("ASCII_"+name,VideoWriter::fourcc('m','p','4','v'),FPS,Size(dim_image.x,dim_image.y),true);

    std::cout << "Writing in progress..." << endl;

    int percent=-1;
    int total_frame = distance(fs::directory_iterator("frame_final_product"), fs::directory_iterator{}); 
    int frame_number=0;
    for(const auto & entry : fs::directory_iterator(pathing)){
        outputVideo.write(imread(pathing+to_string(frame_number)+".jpg"));
        frame_number++;
        if(percent != 100*frame_number/total_frame){
            percent = 100*frame_number/total_frame;
            loading_bar(percent);
        }
    }
    cout << endl;
    return 1;
}