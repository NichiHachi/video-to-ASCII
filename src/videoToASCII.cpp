#include "charToMatrix.h"
#include "videoToFrame.h"
#include "frameToASCII.h"
#include "asciiToFrame.h"
#include "frameToVideo.h"

#include <iostream>

#ifdef _WIN32
    #include <direct.h>
#elif __linux__
    #include <sys/stat.h>
#endif

namespace fs = std::filesystem;

//TODO: Try to change the return value of all function, it sucks like it is now
//TODO: Comment all function and what they do (Also separate everything into small function because you ARE DUMB BITCH WHO CAN CODE)

void createAndCleanDirectory(const std::string& directory){
    #ifdef _WIN32
        _mkdir(directory.c_str());
    #elif __linux__
        mkdir(directory.c_str(), 0777);
    #endif

    for(const auto & entry : fs::directory_iterator(directory)){
        fs::remove(entry);
    }
}

void createDirectories(bool isTricolor){
    createAndCleanDirectory("frame-video");
    createAndCleanDirectory("frame-final-video");

    if(!isTricolor){
        createAndCleanDirectory("frame-ASCII");
    }
    else{
        createAndCleanDirectory("frame-ASCII-color-1");
        createAndCleanDirectory("frame-ASCII-color-2");
    }
}

void videoToASCII(std::string videoPath, int FPS_ASCII_Video, bool isTricolor, int color_sub_selection, int accuracySelection, int FPS_Original_Video){
    createDirectories(isTricolor);

    std::cout << "Char" << std::endl;
    
    //TODO: A reunir dans la même fonction en juste charToMatrix
    if(accuracySelection==3){
        charToMatrix("./char/characters-low");
    } else {
        charToMatrix("./char/characters-normal");
    }

    std::cout << "Char ended" << std::endl;
    std::cout << "videoToFrame" << std::endl;

    videoToFrame(videoPath,FPS_Original_Video,FPS_ASCII_Video);

    std::cout << "videoToFrame ended" << std::endl;
    std::cout << "frameToASCII" << std::endl;

    frameToASCII(accuracySelection,color_sub_selection,isTricolor);

    std::cout << "frameToASCII ended" << std::endl;
    std::cout << "asciiToFrame" << std::endl;
    
    //TODO: A reunir dans la même fonction en juste asciiToFrame
    if (!isTricolor) {
        asciiToFrameBicolor(color_sub_selection);
    } else {
        asciiToFrameTricolor(color_sub_selection);
    }

    std::cout << "asciiToFrame ended" << std::endl;
    std::cout << "frameToVideo" << std::endl;

    frameToVideo(videoPath,FPS_ASCII_Video);

    std::cout << "frameToVideo ended" << std::endl;
}