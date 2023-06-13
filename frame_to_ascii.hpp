#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

using namespace sf;
using namespace std;
namespace fs = filesystem;

const string path_folder_frame_video = "frame_video";
const string path_folder_ascii = "frame_ascii";

void image_bicolor_maping_matrice(char **arr, string path_file, int dim_video_x, int dim_video_y, int dim_chara_x, int dim_chara_y){
    Image image;
    image.loadFromFile(path_file);
    Color color;
    int color_add;
    int x_max = dim_video_x-(dim_video_x%dim_chara_x);
    int y_max = dim_video_y-(dim_video_y%dim_chara_y);
    for(int y=0;y<y_max;y++){
        for(int x=0;x<x_max;x++){
            color = image.getPixel(x,y);
            color_add = color.r+color.g+color.b;
            if(color_add < 192){
                arr[y][x] = '1';
            }
            else{
                arr[y][x] = '0';
            }
        }
    }
}


void bicolor_image_maping_matrice(char **arr, string path_file, int dim_video_x, int dim_video_y, int dim_chara_x, int dim_chara_y){
    Image image;
    image.loadFromFile(path_file);
    Color color;
    int color_add;
    int x_max = dim_video_x-(dim_video_x%dim_chara_x);
    int y_max = dim_video_y-(dim_video_y%dim_chara_y);
    for(int y=0;y<y_max;y++){
        for(int x=0;x<x_max;x++){
            color = image.getPixel(x,y);
            color_add = color.r+color.g+color.b;
            if(color_add < 192){
                arr[y][x] = '2';
            }
            else if(color_add > 573){
                arr[y][x] = '0';
            }
            else{
                arr[y][x] = '1';
            }
        }
    }
}

void bicolor_frame_to_ascii(vector<vector<string>>list_matrice_characters,char **matrice_image, string name_file, int precision, int color_factor, int dim_video_x, int dim_video_y,int dim_x_chara, int dim_y_chara){
    int number_x_box = dim_video_x/dim_x_chara*precision;
    int number_y_box = dim_video_y/dim_y_chara*precision;
    int nbr_charactere = distance(fs::directory_iterator("characters"), fs::directory_iterator{});
    int score;
    int max_score;
    char max_chara;
    bool can_have_better_score;

    fstream outfile(path_folder_ascii+'/'+name_file+".txt");
    fstream table;
    table.open(path_folder_ascii+'/'+name_file+".txt",std::ofstream::out | std::ofstream::trunc);
    table.close();
    table.open(path_folder_ascii+'/'+name_file+".txt",ios::app);

    for(int y_box=0 ; y_box<number_y_box ; y_box++){
        for(int x_box=0 ; x_box<number_x_box ; x_box++){
            max_score = -1000;

            for(int index_chara=0 ; index_chara<nbr_charactere ; index_chara++){
                score = 0;
                can_have_better_score = true;

                for(int y_pixel_in_box=0 ; y_pixel_in_box<dim_y_chara ; y_pixel_in_box++){
                    for(int x_pixel_in_box=0 ; x_pixel_in_box<dim_x_chara ; x_pixel_in_box++){
                        if(matrice_image[(y_pixel_in_box+y_box*dim_y_chara)/precision][(x_pixel_in_box+x_box*dim_x_chara)/precision]==list_matrice_characters[index_chara][1][y_pixel_in_box*dim_x_chara+x_pixel_in_box]){
                            //If both (x,y) pixel is Black substract 2 to the score and if the pixel is white it's -1.
                            score+=(int(list_matrice_characters[index_chara][1][y_pixel_in_box*dim_x_chara+x_pixel_in_box])-47)*color_factor; 
                        }
                        else{
                            //If the (x,y) pixel is black on one and the other it's white, add 2 to the score.
                            score-=2*color_factor;
                        }

                        if(score+(dim_x_chara*(dim_y_chara-y_pixel_in_box-1)+dim_x_chara-1-x_pixel_in_box)*2<max_score){
                            can_have_better_score = false;
                            break;
                        }
                    }
                    //If the perfect score can't be better than max_score, it's useless to continue
                    if(!can_have_better_score){
                        break;
                    }

                }
                if(score>max_score){
                    max_chara = list_matrice_characters[index_chara][0][0];
                    max_score = score;
                }
            }
            table << max_chara ;
        } 
        table<<endl;
    }
    table.close();
}

int frame_to_ascii(int precision, int color_selection, bool isTricolor){
    Image image;
    image.loadFromFile("frame_video/0.jpg");
    Vector2 dim_video = image.getSize();

    Image chara;
    chara.loadFromFile("characters/#.jpg");
    Vector2 dim_chara = chara.getSize();

    vector<vector<string>> character_matrice = txt_to_list();

    int total_frame = distance(fs::directory_iterator(path_folder_frame_video), fs::directory_iterator{});
    int frame_number = 0;

    if(!isTricolor){
        int color_factor; 
        if(color_selection==1){
        color_factor = 1;
        }
        else{
            color_factor = -1;
        }

        int dim_arr_y = dim_video.y-(dim_video.y%dim_chara.y);
        int dim_arr_x = dim_video.x-(dim_video.x%dim_chara.x);
        
        char** arr = new char*[dim_arr_y];
        for (int i=0; i < dim_arr_y; i++) {
            arr[i] = new char[dim_arr_x];
        }

        for(const auto & entry : fs::directory_iterator(path_folder_frame_video)){
            image_bicolor_maping_matrice(arr,path_folder_frame_video+'/'+std::to_string(frame_number)+".jpg",dim_video.x,dim_video.y,dim_chara.x,dim_chara.y);
            bicolor_frame_to_ascii(character_matrice,arr,std::to_string(frame_number),precision,color_factor,dim_video.x,dim_video.y,dim_chara.x,dim_chara.y);
            frame_number++;
            loading_bar(100*frame_number/total_frame);
        }
    }
    else{
        ;
    }
    cout << endl;
    return 1;
}