#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

const std::string file_storage_ascii_path = "../frame-ASCII";
const std::string file_storage_ascii_color_1_path = "../frame-ASCII-color-1";
const std::string file_storage_ascii_color_2_path = "../frame-ascii-color-2";
const std::string file_depot_path = "../frame-final-video";
const int grey_c = 190;
const int white_c = 255;
const int black_c = 0;

void asciiToFrameBicolor(int color_selection){
    TTF_Init();

    int font_size = 15;
    double factor_size = 5.34;

    SDL_Color color_font;
    int color_background;
    if(color_selection==1){
    color_font = {0,0,0};
    color_background = 255;
    }
    else{
    color_font = {255,255,255};
    color_background = 0;
    }

    int image_x;
    int image_y=0;
    std::fstream input(file_storage_ascii_path+"/0.txt");
    for(std::string line; getline(input,line);){
        if(image_y==0){
            image_x = line.length();
        }
        image_y++;
    }

    TTF_Font* font = TTF_OpenFont("../font/consola.ttf",font_size);

    SDL_Rect rect; 
    rect.x = 0;  
    rect.w = image_x*font_size*factor_size/10; 
    
    int frameNumber = 0;
    int frameTotal = std::distance(fs::directory_iterator(file_storage_ascii_path), fs::directory_iterator{});
    for(const auto & entry : fs::directory_iterator(file_storage_ascii_path)){

        SDL_Surface* frame;
        SDL_Surface* text;
        rect.y = 0; 
        rect.h = 40;
        frame = SDL_CreateRGBSurface(SDL_SWSURFACE,image_x*factor_size*font_size/10,image_y*font_size,32,0,0,0,0);
        SDL_FillRect(frame, NULL, SDL_MapRGB(frame->format,color_background, color_background, color_background));

        std::fstream image(file_storage_ascii_path+std::to_string(frameNumber)+".txt");
        for(std::string line; getline(image,line);){
            text = TTF_RenderText_Blended_Wrapped(font,line.c_str(),color_font,image_x*factor_size*font_size/10);
            SDL_BlitSurface(text,&rect,frame,NULL);
            SDL_FreeSurface(text);
            rect.h += font_size; 
            rect.y -= font_size; 
        }
    
        IMG_SaveJPG(frame, (file_depot_path + '/' + std::to_string(frameNumber) + ".jpg").c_str(), 100);
        SDL_FreeSurface(frame);

        frameNumber++;
    }
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}


void asciiToFrameTricolor(int color_selection){
    TTF_Init();

    int font_size=15;
    double factor_size=5.34;

    SDL_Color color_font_1;
    SDL_Color color_font_2;
    int color_background;
    //W - BG
    if(color_selection==1){
    color_background = white_c;
    color_font_1 = {black_c,black_c,black_c};
    color_font_2 = {grey_c,grey_c,grey_c};
    }
    //B - WG
    else if(color_selection==2){
    color_background = black_c;
    color_font_1 = {white_c,white_c,white_c};
    color_font_2 = {grey_c,grey_c,grey_c};
    }
    //G - BW
    else{
        color_background = grey_c;
        color_font_1 = {black_c,black_c,black_c};
        color_font_2 = {white_c,white_c,white_c};
    }

    int image_x;
    int image_y=0;
    std::fstream input(file_storage_ascii_color_1_path+"/0.txt");
    for(std::string line; getline(input,line);){
        if(image_y==0){
            image_x = line.length();
        }
        image_y++;
    }

    Uint32 rmask,gmask,bmask,amask;
    TTF_Font* font = TTF_OpenFont("../font/consola.ttf",font_size);

    SDL_Rect rect; 
    rect.x = 0;  
    rect.w = image_x*font_size*factor_size/10;

    int frameTotal = std::distance(fs::directory_iterator("frame_ascii_color_1"), fs::directory_iterator{});
    for(int frameNumber = 0; frameNumber < frameTotal; frameNumber++){

        SDL_Surface* frame;
        SDL_Surface* text;
        frame = SDL_CreateRGBSurface(SDL_SWSURFACE,image_x*factor_size*font_size/10,image_y*font_size,32,0,0,0,0);
        SDL_FillRect(frame, NULL, SDL_MapRGB(frame->format,color_background, color_background, color_background));

        //TEXT COLOR 1
        rect.y = 0; 
        rect.h = 40;
        std::fstream image_1(file_storage_ascii_color_1_path+std::to_string(frameNumber)+".txt");
        for(std::string line; getline(image_1,line);){
            text = TTF_RenderText_Blended_Wrapped(font,line.c_str(),color_font_1,image_x*factor_size*font_size/10);
            SDL_BlitSurface(text,&rect,frame,NULL);
            SDL_FreeSurface(text);
            rect.h += font_size; 
            rect.y -= font_size; 
        }
        
        //TEXT COLOR 2
        rect.y = 0; 
        rect.h = 40;
        std::fstream image_2(file_storage_ascii_color_2_path+std::to_string(frameNumber)+".txt");
        for(std::string line_2; getline(image_2,line_2);){
            text = TTF_RenderText_Blended_Wrapped(font,line_2.c_str(),color_font_2,image_x*factor_size*font_size/10);
            SDL_BlitSurface(text,&rect,frame,NULL);
            SDL_FreeSurface(text);
            rect.h += font_size; 
            rect.y -= font_size; 
        }
    
        IMG_SaveJPG(frame,(file_depot_path+std::to_string(frameNumber)+".jpg").c_str(),100);
        SDL_FreeSurface(frame);
    }
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
}