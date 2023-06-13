#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std;
namespace fs = filesystem;

const string file_storage_ascii_path = "frame_ascii/";
const string file_depot_path = "frame_final_product/";

int ascii_to_frame(int selection,int color_selection){
    TTF_Init();

    int font_size;
    double factor_size;
    switch(selection){
        case(10):
            font_size = 10;
            factor_size = 6;
            break;
        case(20):
            font_size = 20;
            factor_size = 5.5;
            break;
        default:
            font_size = 30;
            factor_size = 5.67;
            break;
    }

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
    fstream input("frame_ascii/0.txt");
    for(string line; getline(input,line);){
        if(image_y==0){
            image_x = line.length();
        }
        image_y++;
    }

    Uint32 rmask,gmask,bmask,amask;
    TTF_Font* font = TTF_OpenFont("font_sdl2.ttf",font_size);

    SDL_Rect rect; 
    rect.x = 0;  
    rect.w = image_x*font_size*factor_size/10; 

    cout <<  "Transforming ASCII into frames..." << endl;
    int frame_number = 0;
    int total_frame = distance(fs::directory_iterator("frame_ascii"), fs::directory_iterator{});
    for(const auto & entry : fs::directory_iterator(file_storage_ascii_path)){

        SDL_Surface* frame;
        SDL_Surface* texte;
        rect.y = 0; 
        rect.h = 40;
        frame = SDL_CreateRGBSurface(SDL_SWSURFACE,image_x*factor_size*font_size/10,image_y*font_size,32,0,0,0,0);
        SDL_FillRect(frame, NULL, SDL_MapRGB(frame->format,color_background, color_background, color_background));

        fstream image(file_storage_ascii_path+to_string(frame_number)+".txt");
        for(string line; getline(image,line);){
            texte = TTF_RenderText_Blended_Wrapped(font,line.c_str(),color_font,image_x*factor_size*font_size/10);
            SDL_BlitSurface(texte,&rect,frame,NULL);
            SDL_FreeSurface(texte);
            rect.h += font_size; 
            rect.y -= font_size; 
        }
    
        IMG_SaveJPG(frame,(file_depot_path+to_string(frame_number)+".jpg").c_str(),100);
        SDL_FreeSurface(frame);

        frame_number++;
        loading_bar(100*frame_number/total_frame);
    }
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 1;
}